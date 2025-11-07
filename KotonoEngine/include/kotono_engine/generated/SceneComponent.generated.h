#define GENERATED_KSCENECOMPONENT() \
	private: \
		using Base = KObject; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
