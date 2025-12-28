#define GENERATED_KINTERFACECOLLIDERCOMPONENT() \
	private: \
		using Self = KInterfaceColliderComponent; \
		using Base = KInterfaceComponent; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(const nlohmann::json& json) override; \
	private: \
		UPtr<KInterfaceColliderComponent> Ptr() const;
