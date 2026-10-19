import json
from datetime import datetime
from pathlib import Path
from Reflection import Reflector, EngineDirectory

class Generator():
    GeneratedRegistry: Path = EngineDirectory / "Cache/Generator/generated.json"
    GeneratedDirectory: Path = EngineDirectory / "Cache/Generator/Generated"

    def __init__(self) -> None:
        self.reflector = Reflector()
        self.reflector.Reflect()

    def Generate(self) -> None:
        print("Generating...")

        generatedRegistry = {}

        if self.GeneratedRegistry.exists():
            generatedRegistry = json.loads(self.GeneratedRegistry.read_text())

        if "headers" not in generatedRegistry:
            generatedRegistry["headers"] = []

        for result in self.reflector.results:
            time: datetime = datetime.fromtimestamp(result.filePath.stat().st_mtime)
            formattedTime: str = f"{time:%Y-%m-%d-%H:%M:%S}"
            strPath: str = str(result.filePath)

            isInList: bool = False
            for header in generatedRegistry["headers"]:
                if header["path"] != strPath:
                    continue

                if header["modified"] != formattedTime:
                    header["modified"] = formattedTime
                    self.GenerateFiles(result)

                isInList = True
                break

            if not isInList:
                header = {}
                header["path"] = strPath
                header["modified"] = formattedTime
                generatedRegistry["headers"].append(header)
                self.GenerateFiles(result)

        self.GeneratedRegistry.parent.mkdir(parents=True, exist_ok=True)
        self.GeneratedRegistry.write_text(json.dumps(generatedRegistry))

        self.GenerateRegistrator()

    def GenerateFiles(self, result: Reflector.ReflectionResult) -> None:
        self.GenerateHeader(result)
        self.GenerateSource(result)

        print(f"Generated {str(result.filePath)}")

    def GenerateHeader(self, result: Reflector.ReflectionResult) -> None:
        className: str = result.classInfo.name
        baseClass: str = result.classInfo.base

        headerText: str
        if result.classInfo.isAbstract:
            headerText = rf"""
#pragma once

#ifdef GENERATED
#undef GENERATED
#endif

#define GENERATED() \
	private: \
		using Self = {className}; \
		using Base = {baseClass}; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(nlohmann::json const& json) override; \
		void PopulateMemberVariables(std::vector<UVariableInfo>& variables) const override; \
		auto Ptr() const -> UObjectPtr<{className}>; \
		class Class : public Base::Class \
		{{ \
		public: \
			using Base::Class::Instantiate; \
			auto Instantiate() const -> UObjectPtr<KObjectBase> override = 0; \
		}}; \
		auto GetClass() const -> UPtr<KObjectReflectionBase::Class const> override = 0; \
	private:
"""
        else:
            headerText = rf"""
#pragma once

#ifdef GENERATED
#undef GENERATED
#endif

#define GENERATED() \
	private: \
		friend void Register_{className}(); \
		friend void Unregister_{className}(); \
	private: \
		using Self = {className}; \
		using Base = {baseClass}; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(nlohmann::json const& json) override; \
		void PopulateMemberVariables(std::vector<UVariableInfo>& variables) const override; \
		auto Ptr() const -> UObjectPtr<{className}>; \
		class Class : public Base::Class \
		{{ \
		public: \
			using Base::Class::Instantiate; \
			static auto Get() -> UPtr<Class const> const&; \
			auto Instantiate() const -> UObjectPtr<KObjectBase> override; \
		}}; \
		auto GetClass() const -> UPtr<KObjectReflectionBase::Class const> override; \
	private: \
		static UPtr<Class const> class_; \
	private:
"""

        fileName: str = result.filePath.name.replace(".h", ".generated.h")
        filePath: Path = self.GeneratedDirectory / fileName
        filePath.parent.mkdir(parents=True, exist_ok=True)
        filePath.write_text(headerText)

    def GenerateSource(self, result: Reflector.ReflectionResult) -> None:
        className: str = result.classInfo.name

        serializeCode: str = ""
        deserializeCode: str = ""
        variablesCode: str = ""

        for variable in result.variables:
            serializeCode += f"""	USerialize<decltype({variable.name})>{{}}(get(json, "{variable.name}"), {variable.name});\n"""
            deserializeCode += f"""	if (contains(json, "{variable.name}")) UDeserialize<decltype({variable.name})>{{}}(get(json, "{variable.name}"), {variable.name});\n"""
            variablesCode += f"""		{{ "{variable.name}", "{variable.type}", offsetof(Self, {variable.name}) }},"""

        sourceText: str = f"""
void {className}::SerializeTo(nlohmann::json& json) const
{{
	Base::SerializeTo(json);
{serializeCode}
}}

void {className}::DeserializeFrom(nlohmann::json const& json)
{{
	Base::DeserializeFrom(json);
{deserializeCode}
}}

void {className}::PopulateMemberVariables(std::vector<UVariableInfo>& variables) const
{{
	Base::PopulateMemberVariables(variables);
	variables.insert(variables.end(), {{
{variablesCode}
	}});
}}

auto {className}::Ptr() const -> UObjectPtr<{className}>
{{
	return Cast<{className}>(Pointer);
}}
"""

        if not result.classInfo.isAbstract:
            sourceText += f"""
UPtr<{className}::Class const> {className}::class_{{ nullptr }};

auto {className}::Class::Get() -> UPtr<Class const> const&
{{
	return {className}::class_;
}}

auto {className}::Class::Instantiate() const -> UObjectPtr<KObjectBase>
{{
	return UCreate<{className}>{{}}();
}}

auto {className}::GetClass() const -> UPtr<KObjectReflectionBase::Class const>
{{
	return class_;
}}

void Register_{className}()
{{
	SObjectFactory::Register("{className}", +[]() static -> UObjectPtr<KObject> {{ return UCreate<{className}>{{}}(); }});
	{className}::class_ = UPtr{{ new {className}::Class{{}} }};
}}

void Unregister_{className}()
{{
	delete {className}::class_.Get();
}}
"""


        fileName: str = result.filePath.name.replace(".h", ".generated.inl")
        filePath: Path = self.GeneratedDirectory / fileName
        filePath.parent.mkdir(parents=True, exist_ok=True)
        filePath.write_text(sourceText)

    def GenerateRegistrator(self) -> None:
        registerFunctionsCode: str = ""
        unregisterFunctionsCode: str = ""
        registerCallCode: str = ""
        unregisterCallCode: str = ""

        for result in self.reflector.results:
            if not result.classInfo.isAbstract:
                registerFunctionsCode += f"extern void Register_{result.classInfo.name}();\n"
                unregisterFunctionsCode += f"extern void Unregister_{result.classInfo.name}();\n"
                registerCallCode += f"	Register_{result.classInfo.name}();\n"
                unregisterCallCode += f"	Unregister_{result.classInfo.name}();\n"

        generatedCode: str = rf"""
{registerFunctionsCode}

{unregisterFunctionsCode}

void RegisterObjectClasses()
{{
{registerCallCode}
}}

void UnregisterObjectClasses()
{{
{unregisterCallCode}
}}
"""

        filePath: Path = self.GeneratedDirectory / "ClassRegistrator.generated.inl"
        filePath.parent.mkdir(parents=True, exist_ok=True)
        filePath.write_text(generatedCode)

        print("Generated class registrator")

if __name__ == "__main__":
    Generator().Generate()
