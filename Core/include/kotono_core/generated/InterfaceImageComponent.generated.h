#define GENERATED_KINTERFACEIMAGECOMPONENT() \
	private: \
		using Base = KInterfaceComponent; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
		void DeserializeFrom(const nlohmann::json& json) override; \
	private: \
		UPtr<KInterfaceImageComponent> Ptr() const;
