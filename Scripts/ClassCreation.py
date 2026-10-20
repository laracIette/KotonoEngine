import json
import tkinter
from pathlib import Path
from Reflection import Reflector, ReflectRegistry, EngineDirectory
from tkinter import filedialog

root = tkinter.Tk()
root.withdraw()

class ClassInfos():
    def __init__(self, filePath: Path, name: str) -> None:
        self.filePath = filePath
        self.name = name

class ClassCreator():
    def __init__(self) -> None:
        self.reflector = Reflector()
        self.reflector.Reflect()

    def CreateClass(self) -> None:
        baseClassInfos: ClassInfos = self.GetBaseClassInfos()
        newClassName: str = self.GetNewClassName()

        projectDirectory: Path = self.GetProjectDirectory()

        headerDirectory: Path = self.GetHeaderDirectory(projectDirectory)
        sourceDirectory: Path = self.GetSourceDirectory(projectDirectory)


        headerPath: Path = self.WriteHeaderFile(baseClassInfos, newClassName, headerDirectory)
        sourcePath: Path = self.WriteSourceFile(newClassName, sourceDirectory)

        self.WriteReflectRegistry(headerPath)

        self.UpdateProjectCMakeClasses(headerPath, sourcePath, projectDirectory)

    def GetBaseClassInfos(self) -> ClassInfos:
        while True:
            baseClassName: str = self.GetClassName("The name of the base class (with prefix): ")

            for result in self.reflector.results:
                if result.classInfo.name == baseClassName:
                    return ClassInfos(result.filePath, baseClassName)

            print("Please provide an existing class name.")

    def GetNewClassName(self) -> str:
        return self.GetClassName("The name of the new class (without prefix): ")

    def GetClassName(self, inputText: str) -> str:
        while True:
            result = input(inputText)
            if result and not result[0].isdigit() and result.isalnum():
                return result
            else:
                print("Class names only accept letters and numbers and can't start with a number.")

    def GetProjectDirectory(self) -> Path:
        print("Select the directory of the project to add a class to")
        return Path(filedialog.askdirectory(title="Project directory", initialdir=EngineDirectory)).resolve()

    def GetHeaderDirectory(self, projectDirectory: Path) -> Path:
        print("Select a directory for the class's header file")
        return Path(filedialog.askdirectory(title="Header file directory", initialdir=projectDirectory)).resolve()

    def GetSourceDirectory(self, projectDirectory: Path) -> Path:
        print("Select a directory for the class's source file")
        return Path(filedialog.askdirectory(title="Source file directory", initialdir=projectDirectory)).resolve()

    def WriteHeaderFile(self, baseClassInfos: ClassInfos, newClassName: str, directory: Path) -> Path:
        prefix: str = baseClassInfos.name[0]
        fileName: str = f"{newClassName}.h"
        filePath: Path = directory / fileName

        baseClassFileName: str = baseClassInfos.filePath.name
        baseClassDirectory: str = baseClassInfos.filePath.parent.name

        filePath.write_text(
f"""#pragma once
#include "{baseClassDirectory}/{baseClassFileName}"

#include "{newClassName}.generated.h"

class {prefix}{newClassName} final : public {baseClassInfos.name}
{{
    GENERATED()

public:

private:

}};
"""
        )

        print(f"Wrote header file at {str(filePath)}")

        return filePath

    def WriteSourceFile(self, newClassName: str, directory: Path) -> Path:
        fileName: str = f"{newClassName}.cpp"
        filePath: Path = directory / fileName

        headerName: str = f"{newClassName}.h"

        filePath.write_text(
f"""#include "{headerName}"

#include "{newClassName}.generated.inl"
"""
        )

        print(f"Wrote source file at {str(filePath)}")

        return filePath

    def WriteReflectRegistry(self, headerPath: Path) -> None:
        registry: ReflectRegistry = self.reflector.ReadReflectRegistry()

        for project in registry.projects:
            if str(headerPath).startswith(str(project.directory)):
                project.files.append(headerPath)

        self.reflector.WriteReflectRegistry(registry)

        print(f"Wrote {headerPath} into reflect registry")

    def UpdateProjectCMakeClasses(self, headerPath: Path, sourcePath: Path, projectDirectory: Path) -> None:

        cmakeClassesPath: Path = projectDirectory / "ObjectClasses.cmake"
        result: str = cmakeClassesPath.read_text()

        headerRelative: str = str(headerPath).removeprefix(str(projectDirectory))
        headerRelative = Path(headerRelative).as_posix().removeprefix("/")

        sourceRelative: str = str(sourcePath).removeprefix(str(projectDirectory))
        sourceRelative = Path(sourceRelative).as_posix().removeprefix("/")

        if headerRelative.startswith("Public"):
            result = self.InsertLine(result, "set(CLASS_PUBLIC_HEADERS\n", f"""    "{headerRelative}"\n""")
        elif headerRelative.startswith("Private"):
            result = self.InsertLine(result, "set(CLASS_PRIVATE_HEADERS\n", f"""    "{headerRelative}"\n""")

        result = self.InsertLine(result, "set(CLASS_SOURCES\n", f"""    "{sourceRelative}"\n""")

        cmakeClassesPath.write_text(result)

    def InsertLine(self, text: str, previousLine: str, insertLine: str) -> str:
        result: str = ""

        foundLine: bool = False
        for line in text.splitlines(keepends=True):
            result += line

            if foundLine:
                continue

            if line == previousLine:
                result += insertLine
                foundLine = True

        return result

if __name__ == "__main__":
    ClassCreator().CreateClass()
