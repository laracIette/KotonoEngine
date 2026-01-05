#define GENERATED_KSCENEMESHCOMPONENT() \
	private: \
		using Self = KSceneMeshComponent; \
		using Base = KSceneComponent; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(const nlohmann::json& json) override; \
		std::vector<UVariableInfo> GetMemberVariables() const override; \
	private: \
		UPtr<KSceneMeshComponent> Ptr() const;
