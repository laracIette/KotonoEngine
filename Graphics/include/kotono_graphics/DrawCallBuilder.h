#pragma once
struct UDrawCall;
struct UDrawDataBufferData;
struct UTransformBufferData;
struct UParametersBufferData;
struct UDrawCallBuilder final
{
public:
	UDrawCallBuilder();
	~UDrawCallBuilder();

	void Register();
	void Unregister();

	UDrawCall* GetDrawCall() const;
	UDrawDataBufferData* GetDrawData() const;
	UTransformBufferData* GetTransform() const;
	UParametersBufferData* GetParameters() const;

private:
	UDrawCall* drawCall_;
	bool isRegistered_;

	UDrawDataBufferData* drawData_;
	UTransformBufferData* transform_;
	UParametersBufferData* parameters_;
};