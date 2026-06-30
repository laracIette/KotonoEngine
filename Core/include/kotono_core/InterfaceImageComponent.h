#pragma once
#include "generated/InterfaceImageComponent.generated.h"
#include "InterfaceComponent.h"
class UShader;
class UTexture;
class KInterfaceImageComponent : public KInterfaceComponent
{
	GENERATED_KINTERFACEIMAGECOMPONENT()

public:
	KInterfaceImageComponent();
	~KInterfaceImageComponent() override;

protected:
	void Init() override;

public:
	UAsset<UShader> GetShader() const;
	UAsset<UTexture> GetTexture() const;

	UEvent<>& GetEventShaderUpdated();
	UEvent<>& GetEventTextureUpdated();

	void SetShader(UAsset<UShader> shader);
	void SetTexture(UAsset<UTexture> texture);

	void Spawn() override;

private:
	UAsset<UShader> shader_;
	UAsset<UTexture> texture_;
	UEvent<> eventShaderUpdated_;
	UEvent<> eventTextureUpdated_;
};

