#include <ImGUI/imgui.h>
#include "engine/TextUtils.h"
#include "engine/Utilities.h"
#include "engine/Console.h"
#include "engine/Game.h"
#include "Game.h"
#include "Camera.h"

static void DoCamera()
{
	if (ImGui::Begin("Camera"))
	{
		auto first = MainCamera->FirstPerson();

		ImGui::SeparatorText(first ? "Position" : "Target");
		{
			auto& tar = MainCamera->GetTarget();
			if (ImGui::DragFloat("X", &tar.x, 1.0, -50, 50))
				MainCamera->Update();
			if (ImGui::DragFloat("Y", &tar.y, 1.0, -50, 50))
				MainCamera->Update();
			if (ImGui::DragFloat("Z", &tar.z, 1.0, -100, 100))
				MainCamera->Update();
		}

		ImGui::SeparatorText("Angles");
		{
			auto& ang = MainCamera->GetAngles();
			if (ImGui::DragFloat("Roll", &ang.x, 1.0, -359, 359))
				MainCamera->Update();
			if (ImGui::DragFloat("Pitch", &ang.y, 1.0, -359, 359))
				MainCamera->Update();
			if (ImGui::DragFloat("Yaw", &ang.z, 1.0, -359, 359))
				MainCamera->Update();
		}

		ImGui::Separator();
		if (first) ImGui::BeginDisabled();
		if (ImGui::DragFloat("Distance", &MainCamera->GetDistance(), 1.0, -100, 100, "%.3f", ImGuiSliderFlags_Logarithmic))
			MainCamera->Update();
		if (first) ImGui::EndDisabled();


		ImGui::SeparatorText("Settings");
		ImGui::Checkbox("Locked", &MainCamera->Locked);
		if (ImGui::Checkbox("First", &first))
			MainCamera->FirstPerson(first);

		if (ImGui::Button("Reset"))
		{
			MainCamera->Target(glm::vec3(0, 0, 0));
			MainCamera->Angles(glm::vec3(0, 46, 0));
			MainCamera->Distance(70);
			MainCamera->Update();
		}

		/*
		if (ImGui::Button("Copy JSON"))
		{
			std::string json;
			json += "{\n";
			auto tar = MainCamera->GetTarget();
			auto ang = MainCamera->GetAngles();
			auto dis = MainCamera->GetDistance();
			json += fmt::format("\t\"target\": [{}, {}, {}],\n", tar[0], tar[1], tar[2]);
			json += fmt::format("\t\"angles\": [{}, {}, {}],\n", ang[0], ang[1], ang[2]);
			json += fmt::format("\t\"distance\": {},\n", dis);
			json += fmt::format("\t\"drum\": {},\n", commonUniforms.CurveEnabled == 1);
			json += fmt::format("\t\"drumAmount\": {},\n", commonUniforms.CurveAmount);
			json += fmt::format("\t\"drumPower\": {}\n", commonUniforms.CurvePower);
			json += "}\n";
			ImGui::SetClipboardText(json.c_str());
		}
		ImGui::SameLine();
		if (ImGui::Button("Paste JSON"))
		{
			try
			{
				auto json = json5pp::parse5(ImGui::GetClipboardText());
				LoadCamera(json);
			}
			catch (std::runtime_error& x)
			{
				conprint(4, x.what());
			}
		}
		*/
	}
	ImGui::End();
}

static void DoLights()
{
	static const char lightLabels[] = "S\0" "2\0" "3\0" "4\0" "5\0" "6\0" "7\0" "8\0";
	if (ImGui::Begin("Lighting"))
	{
		static int lightIndex = 0;
		for (int i = 0; i < MaxLights; i++)
		{
			bool selected = lightIndex == i;
			if (ImGui::RadioButton(lightLabels + (i * 2), selected))
			{
				lightIndex = i;
			}
			if (i % (MaxLights / 2) != (MaxLights / 2) - 1)
				ImGui::SameLine();
		}

		auto& l = commonUniforms.Lights[lightIndex];

		ImGui::Text("Position");
		{
			ImGui::DragFloat("X", &l.pos.x, 1.0, -50, 50);
			ImGui::DragFloat("Y", &l.pos.y, 1.0, -50, 50);
			ImGui::DragFloat("Z", &l.pos.z, 1.0, -50, 50);

			if (ImGui::Button("Reset"))
			{
				if (lightIndex == 0)
				{
					l.pos = { 0.0f, 15.0f, 20.0f, 0 };
					l.color = { 1.0f, 1.0f, 1.0f, 0.25f };
				}
				else
				{
					l.pos = { 0, 0, 0, 0 };
					l.color = { 0, 0, 0, 0 };
				}
			}
		}

		ImGui::ColorPicker4("Color", &l.color.x, ImGuiColorEditFlags_DisplayRGB);

		ImGui::Checkbox("Fresnel", &commonUniforms.Fresnel);
		ImGui::Checkbox("Toon", &commonUniforms.Toon);

		/*
		if (ImGui::Button("Copy JSON"))
		{
			std::string json;
			std::vector<std::string> blocks;
			json += "[\n";
			for (int i = 0; i < MaxLights; i++)
			{
				l = commonUniforms.Lights[i];
				if (l.color[3] == 0)
					continue;
				std::string block;
				block += "\t{\n";
				block += fmt::format("\t\t\"pos\": [{}, {}, {}, {}],\n", l.pos[0], l.pos[1], l.pos[2], l.pos[3]);
				block += fmt::format("\t\t\"col\": [{}, {}, {}, {}] \n", l.color[0], l.color[1], l.color[2], l.color[3]);
				block += "\t}";
				blocks.push_back(block);
			}
			json += StringJoin(blocks.begin(), blocks.end(), ",\n", "\n");
			json += "]\n";
			ImGui::SetClipboardText(json.c_str());
		}
		ImGui::SameLine();
		if (ImGui::Button("Paste JSON"))
		{
			try
			{
				auto json = json5pp::parse5(ImGui::GetClipboardText());
				LoadLights(json);
			}
			catch (std::runtime_error& x)
			{
				conprint(4, x.what());
			}
		}
		*/
	}
	ImGui::End();
}

extern Tickable root;
Tickable* tickable = &root;
int blankIDs = 0;

static void TraverseTree(Tickable* origin)
{
	int flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
	if (origin->size() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;
	if (tickable == origin)
		flags |= ImGuiTreeNodeFlags_Selected;
	
	auto name = std::string(origin->ID);
	if (name.empty())
		name = fmt::format("{}", blankIDs);
	name += fmt::format("##{}", blankIDs);
	blankIDs++;

	if (ImGui::TreeNodeEx(name.c_str(), flags))
	{
		if (ImGui::IsItemClicked())
		{
			tickable = origin;
		}
		for (int i = 0; i < origin->size(); i++)
		{
			TraverseTree(origin->operator[](i));
		}
		ImGui::TreePop();
	}
}

static void DoSceneTree()
{
	if (ImGui::Begin("Scene Tree"))
	{
		ImGui::BeginChild("left pane", ImVec2(200, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		{
			blankIDs = 0;
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 4);
			TraverseTree(&root);
			ImGui::PopStyleVar();
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("item view", ImVec2(0, 0)); //-ImGui::GetFrameHeightWithSpacing()));
		{
			if (auto tgen = dynamic_cast<Tickable*>(tickable))
			{
				ImGui::SeparatorText("Tickable");
				ImGui::Checkbox("Visible", &tgen->Visible);
				ImGui::Checkbox("Enabled", &tgen->Enabled);
			}

			if (auto t2d = dynamic_cast<Tickable2D*>(tickable))
			{
				ImGui::SeparatorText("Tickable2D");
				ImGui::DragFloat2("Position", &t2d->Position.x);
			}

			if (auto cam = dynamic_cast<Camera*>(tickable))
			{
				ImGui::SeparatorText("Camera");
				auto first = cam->FirstPerson();
				ImGui::Text(first ? "Position" : "Target");
				{
					auto& tar = cam->GetTarget();
					if (ImGui::DragFloat("X", &tar.x, 1.0, -50, 50))
						cam->Update();
					if (ImGui::DragFloat("Y", &tar.y, 1.0, -50, 50))
						cam->Update();
					if (ImGui::DragFloat("Z", &tar.z, 1.0, -100, 100))
						cam->Update();
				}

				ImGui::Text("Angles");
				{
					auto& ang = cam->GetAngles();
					if (ImGui::DragFloat("Roll", &ang.x, 1.0, -359, 359))
						cam->Update();
					if (ImGui::DragFloat("Pitch", &ang.y, 1.0, -359, 359))
						cam->Update();
					if (ImGui::DragFloat("Yaw", &ang.z, 1.0, -359, 359))
						cam->Update();
				}

				ImGui::Separator();
				if (first) ImGui::BeginDisabled();
				if (ImGui::DragFloat("Distance", &cam->GetDistance(), 1.0, -100, 100, "%.3f", ImGuiSliderFlags_Logarithmic))
					cam->Update();
				if (first) ImGui::EndDisabled();


				ImGui::Text("Settings");
				ImGui::Checkbox("Locked", &cam->Locked);
				if (ImGui::Checkbox("First", &first))
					cam->FirstPerson(first);

				if (ImGui::Button("Reset"))
				{
					cam->Target(glm::vec3(0, 0, 0));
					cam->Angles(glm::vec3(0, 46, 0));
					cam->Distance(70);
					cam->Update();
				}
			}

			if (auto spr = dynamic_cast<SimpleSprite*>(tickable))
			{
				ImGui::SeparatorText("SimpleSprite");
				ImGui::InputInt("Frame", &spr->Frame);
				ImGui::DragFloat("Scale", &spr->ImgScale);
				ImGui::ColorEdit4("Color", &spr->Color.r);
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void Game::ImGui()
{
	DoCamera();
	DoLights();
	DoSceneTree();
}
