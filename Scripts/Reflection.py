import json
import re
from pathlib import Path

EngineDirectory: Path = Path(__file__).resolve().parent.parent
ReflectRegistryPath: Path = EngineDirectory / "reflect.json"

class VariableInfo():
    def __init__(self) -> None:
        self.type: str = ""
        self.name: str = ""

class ClassInfo():
    def __init__(self) -> None:
        self.isAbstract: bool = False
        self.base: str = ""
        self.name: str = ""

class ReflectRegistry():
    class Project():
        def __init__(self, directory: Path, files: list[Path]) -> None:
            self.directory = directory
            self.files = files

    def __init__(self, projects: list[Project]) -> None:
        self.projects = projects

class ReflectionResult():
    def __init__(self, filePath: Path, classInfo: ClassInfo, variables: list[VariableInfo]) -> None:
        self.filePath = filePath
        self.classInfo = classInfo
        self.variables = variables

class Reflector():
    def __init__(self) -> None:
        self.results: list[ReflectionResult] = []

    def Reflect(self) -> None:
        registry: ReflectRegistry = self.ReadReflectRegistry()

        for project in registry.projects:
            for file in project.files:
                self.results.append(self.ReflectFile(file))

    def ReadReflectRegistry(self) -> ReflectRegistry:
        registryContent: str = ReflectRegistryPath.read_text()
        registryJson = json.loads(registryContent)

        projects: list[ReflectRegistry.Project] = []

        if "projects" not in registryJson:
            registryJson["projects"] = []

        for project in registryJson["projects"]:
            directoryJson: str = project["directory"]
            filesJson: list[str] = project["files"]

            directory: Path = Path(directoryJson.replace("${ENGINE_DIRECTORY}", str(EngineDirectory))).resolve()
            files: list[Path] = []

            for file in filesJson:
                filePath = directory / file
                files.append(filePath.resolve())

            projects.append(ReflectRegistry.Project(directory, files))

        return ReflectRegistry(projects)

    def WriteReflectRegistry(self, registry: ReflectRegistry) -> None:
        registryJson = {}
        registryJson["projects"] = []

        for project in registry.projects:
            projectJson = {}

            directory: str = str(project.directory)
            directory = directory.replace(str(EngineDirectory), "${ENGINE_DIRECTORY}")

            projectJson["directory"] = Path(directory).as_posix()
            projectJson["files"] = []

            for file in project.files:
                filePath: str = str(file).removeprefix(str(project.directory))
                filePath = Path(filePath).as_posix().removeprefix("/")

                projectJson["files"].append(filePath)

            registryJson["projects"].append(projectJson)

        ReflectRegistryPath.write_text(json.dumps(registryJson, indent=4))

    def ReflectFile(self, filePath: Path) -> ReflectionResult:
        fileContent: str = filePath.read_text()

        classInfo = self.ReflectClass(fileContent)
        variables = self.ReflectClassVariables(fileContent)

        return ReflectionResult(filePath, classInfo, variables)

    def ReflectClass(self, fileContent: str) -> ClassInfo:
        match = re.search(
            r"(ABSTRACT\s+)?class\s+([a-zA-Z_]\w*)\s*(?:final)?\s*(?::\s*public\s+([a-zA-Z_]\w*)\s*(?:<[^>]*>)?(?:\s*,\s*[^{]+)?)?\{",
            fileContent
        )

        classInfo = ClassInfo()

        if match:
            if match.group(1) is not None:
                classInfo.isAbstract = True

            if match.group(2) is not None:
                classInfo.name = match.group(2)

            if match.group(3) is not None:
                classInfo.base = match.group(3)

        return classInfo

    def ReflectClassVariables(self, fileContent: str) -> list[VariableInfo]:
        matches = re.finditer(
            r"SERIALIZE\s*(?:\(\s*\))?\s*(?:(?:Writable|Readonly|State)Property\s*\(\s*)?([A-Za-z_][\w:]*(?:<[\w:\s<>,]+>)?)\s*(?:,\s*|\s+)([A-Za-z_]\w*)[^;]*;",
            fileContent
        )

        variables = list[VariableInfo]()

        for match in matches:
            variable = VariableInfo()
            variable.type = match.group(1)
            variable.name = match.group(2)
            variables.append(variable)

        return variables
