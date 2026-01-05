#define GENERATED_RINTERFACEOBJECT() \
	private: \
		using Self = RInterfaceObject; \
		using Base = KObject; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(const nlohmann::json& json) override; \
		std::vector<UVariableInfo> GetMemberVariables() const override; \
	private: \
		UPtr<RInterfaceObject> Ptr() const;
