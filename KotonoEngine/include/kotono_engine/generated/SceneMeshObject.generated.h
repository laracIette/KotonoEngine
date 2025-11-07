#define GENERATED_TSCENEMESHOBJECT() \
	private: \
		using Base = TSceneObject; \
		using Base::Base; \
	public: \
		void SerializeTo(nlohmann::json& json) const override; \
