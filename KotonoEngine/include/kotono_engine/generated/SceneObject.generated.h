#define GENERATED_TSCENEOBJECT() \
	private: \
		using Base = KObject; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
