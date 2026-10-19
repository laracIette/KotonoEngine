import tkinter
from pathlib import Path
from Reflection import Reflector
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

        headerDirectory: Path = self.GetHeaderDirectory()
        sourceDirectory: Path = self.GetSourceDirectory()

        self.WriteHeaderFile(baseClassInfos, newClassName, headerDirectory)
        self.WriteSourceFile(newClassName, sourceDirectory)

    def GetBaseClassInfos(self) -> ClassInfos:
        while True:
            baseClassName: str = self.GetClassName("The name of the base class: ")

            for result in self.reflector.results:
                if (result.classInfo.name == baseClassName):
                    return ClassInfos(result.filePath, baseClassName)

            print("Please provide an existing class name.")

    def GetNewClassName(self) -> str:
        return self.GetClassName("The name of the new class: ")

    def GetClassName(self, inputText: str) -> str:
        while True:
            result = input(inputText)
            if result and not result[0].isdigit() and result.isalnum():
                return result
            else:
                print("Class names only accept letters and numbers and can't start with a number.")

    def GetHeaderDirectory(self) -> Path:
        print("Select a directory for the class's header file")
        return Path(filedialog.askdirectory(title="Select a directory for the class's header file")).resolve()

    def GetSourceDirectory(self) -> Path:
        print("Select a directory for the class's source file")
        return Path(filedialog.askdirectory(title="Select a directory for the class's source file")).resolve()

    def WriteHeaderFile(self, baseClassInfos: ClassInfos, newClassName: str, directory: Path) -> None:
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

    def WriteSourceFile(self, newClassName: str, directory: Path) -> None:
        fileName: str = f"{newClassName}.cpp"
        filePath: Path = directory / fileName

        headerName: str = f"{newClassName}.h"

        filePath.write_text(
f"""#include "{headerName}"

#include "{newClassName}.generated.inl"
"""
        )

        print(f"Wrote source file at {str(filePath)}")

ClassCreator().CreateClass()
