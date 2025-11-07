#define GENERATED_KSCENEMESHCOMPONENT() \
	private: \
		using Base = KSceneComponent; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(const nlohmann::json& json) override; \
	private:
