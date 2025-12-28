#define GENERATED_KOBJECT() \
	private: \
		using Self = KObject; \
	public: \
		virtual void SerializeTo(nlohmann::json& json) const; \
		virtual void DeserializeFrom(const nlohmann::json& json); \
	private: \
		UPtr<KObject> Ptr() const;
