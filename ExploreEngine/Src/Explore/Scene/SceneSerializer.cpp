#include "enginepch.h"
#include "SceneSerializer.h"
#include "Components.h"
#include "yaml-cpp/yaml.h"

namespace YAML
{
	//glm::vec3 specialization
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& value)
		{
			Node node;
			node.push_back(value.x);
			node.push_back(value.y);
			node.push_back(value.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& value)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}

			value.x = node[0].as<float>();
			value.y = node[1].as<float>();
			value.z = node[2].as<float>();
			return true;
		}
	};

	//glm::vec4 specialization
	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& value)
		{
			Node node;
			node.push_back(value.x);
			node.push_back(value.y);
			node.push_back(value.z);
			node.push_back(value.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& value)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}

			value.x = node[0].as<float>();
			value.y = node[1].as<float>();
			value.z = node[2].as<float>();
			value.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Explore
{
	YAML::Emitter& operator<< (YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<< (YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z  << v.w << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.operator int();
		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent" << YAML::BeginMap;
			auto& tag = entity.GetComponent<TagComponent>().TagName;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent" << YAML::BeginMap;
			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent" << YAML::BeginMap;
			auto& cc = entity.GetComponent<CameraComponent>();
			auto& camera = cc.Camera;
			out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap; 
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFov" << YAML::Value << camera.GetPerspectiveFov();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNear();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetOrthographicFar();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthophicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNear();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFar();
			out << YAML::EndMap;
			out << YAML::Key << "Primary" << YAML::Value << cc.bPrimary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.bFixedAspectRatio;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<SpriteRenderedComponent>())
		{
			out << YAML::Key << "SpriteRenderedComponent" << YAML::BeginMap;
			auto& sc = entity.GetComponent<SpriteRenderedComponent>();
			out << YAML::Key << "Color" << YAML::Value << sc.Color;
			out << YAML::EndMap;
		}
		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](entt::entity ent) {
			Entity entity = { ent, m_Scene.get() };
			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
		fout.close();
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream fin(filepath);
		std::stringstream strstream;
		strstream << fin.rdbuf();

		YAML::Node data = YAML::Load(strstream.str());
		if (!data["Scene"])
		{
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		EXPLORE_CLIENT_LOG("trace", "Deserialize Scene: {0}", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			//loop entity data from YAML
			for (auto entity : entities)
			{
				int32_t uuid = entity["Entity"].as<int32_t>();
				
				std::string name;
				auto tagcomponent = entity["TagComponent"];
				if (tagcomponent)
				{
					name = tagcomponent["Tag"].as<std::string>();
				}
				EXPLORE_CLIENT_LOG("trace", "Deserialize Entity with Id = {0}, Name = {1}", uuid, name);

				Entity SerializeEntity = m_Scene->CreateEntity(name);

				if (auto transfromcomponent = entity["TransformComponent"])
				{
					auto& tc = SerializeEntity.GetComponent<TransformComponent>();
					tc.Translation = transfromcomponent["Translation"].as<glm::vec3>();
					tc.Rotation = transfromcomponent["Rotation"].as<glm::vec3>();
					tc.Scale = transfromcomponent["Scale"].as<glm::vec3>();
				}

				if (auto cameracomponent = entity["CameraComponent"])
				{
					auto& cc = SerializeEntity.AddComponent<CameraComponent>();
					auto cameraProps = cameracomponent["Camera"];
					cc.Camera.SetProjectionType(SceneCamera::ProjectionType(cameraProps["ProjectionType"].as<int>()));
					cc.Camera.SetPerspectiveFov(cameraProps["PerspectiveFov"].as<float>());
					cc.Camera.SetPerspectiveNear(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFar(cameraProps["PerspectiveFar"].as<float>());
					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNear(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFar(cameraProps["OrthographicFar"].as<float>());
					cc.bPrimary = cameracomponent["Primary"].as<bool>();
					cc.bFixedAspectRatio = cameracomponent["FixedAspectRatio"].as<bool>();
				}

				if (auto spritecomponent = entity["SpriteRenderedComponent"])
				{
					auto& sc = SerializeEntity.AddComponent<SpriteRenderedComponent>();
					sc.Color = spritecomponent["Color"].as<glm::vec4>();
				}
			}
		}
	}
}