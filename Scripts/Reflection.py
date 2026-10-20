import json
import re
from pathlib import Path

EngineDirectory: Path = Path(__file__).resolve().parent.parent

class VariableInfo():
    def __init__(self) -> None:
        self.type: str = ""
        self.name: str = ""

class ClassInfo():
    def __init__(self) -> None:
        self.isAbstract: bool = False
        self.base: str = ""
        self.name: str = ""


class Reflector():
    ReflectRegistryPath: Path = EngineDirectory / "reflect.json"

    class ReflectionResult():
        def __init__(self, filePath: Path, classInfo: ClassInfo, variables: list[VariableInfo]) -> None:
            self.filePath = filePath
            self.classInfo = classInfo
            self.variables = variables

    def __init__(self) -> None:
        self.results: list[Reflector.ReflectionResult] = []

    def Reflect(self) -> None:
        registryContent: str = self.ReflectRegistryPath.read_text()

        files: list[str] = json.loads(registryContent)["files"]

        for file in files:
            file = file.replace("${ENGINE_DIRECTORY}", str(EngineDirectory))
            self.results.append(self.ReflectFile(Path(file)))

    def ReflectFile(self, filePath: Path) -> ReflectionResult:
        fileContent: str = filePath.read_text()

        classInfo = self.ReflectClass(fileContent)
        variables = self.ReflectClassVariables(fileContent)

        return self.ReflectionResult(filePath, classInfo, variables)

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
