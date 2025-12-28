#define GENERATED_KINTERFACEBUTTONCOMPONENT() \
	private: \
		using Self = KInterfaceButtonComponent; \
		using Base = KInterfaceComponent; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(const nlohmann::json& json) override; \
	private: \
		UPtr<KInterfaceButtonComponent> Ptr() const;
