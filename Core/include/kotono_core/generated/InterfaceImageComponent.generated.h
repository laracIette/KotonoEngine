#define GENERATED_KINTERFACEIMAGECOMPONENT() \
	private: \
		using Self = KInterfaceImageComponent; \
		using Base = KInterfaceComponent; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(const nlohmann::json& json) override; \
		std::vector<UVariableInfo> GetMemberVariables() const override; \
	private: \
		UPtr<KInterfaceImageComponent> Ptr() const;
