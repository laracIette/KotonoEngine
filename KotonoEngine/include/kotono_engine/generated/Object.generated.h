#define GENERATED_KOBJECT() \
	public: \
		virtual void SerializeTo(nlohmann::json& json) const; \
