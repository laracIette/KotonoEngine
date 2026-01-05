#define GENERATED_KINTERFACE() \
	private: \
		using Self = KInterface; \
		using Base = KObject; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(const nlohmann::json& json) override; \
		std::vector<UVariableInfo> GetMemberVariables() const override; \
	private: \
		UPtr<KInterface> Ptr() const;
