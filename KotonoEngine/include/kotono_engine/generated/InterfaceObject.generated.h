#define GENERATED_RINTERFACEOBJECT() \
	private: \
		using Base = KObject; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(const nlohmann::json& json) override; \
	private:
