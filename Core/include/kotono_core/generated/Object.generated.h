#define GENERATED_KOBJECT() \
	private: \
		using Self = KObject; \
	public: \
		virtual void SerializeTo(nlohmann::json& json) const; \
		virtual void DeserializeFrom(const nlohmann::json& json); \
		virtual std::vector<UVariableInfo> GetMemberVariables() const; \
	private: \
		UPtr<KObject> Ptr() const;
