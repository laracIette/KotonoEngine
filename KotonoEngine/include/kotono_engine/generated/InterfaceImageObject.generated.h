#define GENERATED_RINTERFACEIMAGEOBJECT() \
	private: \
		using Base = RInterfaceObject; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
