#define GENERATED_KINTERFACEBOXCOMPONENT() \
	private: \
		using Base = KInterfaceComponent; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
