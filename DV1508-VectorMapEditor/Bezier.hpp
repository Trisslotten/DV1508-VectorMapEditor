#pragma once
#include <string>
#include <iostream>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Bezier {
	template<int steps>
	void bezierTable(ImVec2 P[4], ImVec2 results[steps + 1]) {
		static float C[(steps + 1) * 4], *K = 0;
		if (!K) {
			K = C;
			for (unsigned step = 0; step <= steps; ++step) {
				float t = (float)step / (float)steps;
				C[step * 4 + 0] = (1 - t)*(1 - t)*(1 - t);
				C[step * 4 + 1] = 3 * (1 - t)*(1 - t) * t;
				C[step * 4 + 2] = 3 * (1 - t) * t*t;
				C[step * 4 + 3] = t * t*t;
			}
		}
		for (unsigned step = 0; step <= steps; ++step) {
			ImVec2 point = {
				K[step * 4 + 0] * P[0].x + K[step * 4 + 1] * P[1].x + K[step * 4 + 2] * P[2].x + K[step * 4 + 3] * P[3].x,
				K[step * 4 + 0] * P[0].y + K[step * 4 + 1] * P[1].y + K[step * 4 + 2] * P[2].y + K[step * 4 + 3] * P[3].y
			};
			results[step] = point;
		}
	}


	int bezier(float P[4]) {

		const int segments = 64;

		ImGuiStyle style = ImGui::GetStyle();
		ImGuiIO io = ImGui::GetIO();
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		int changed = ImGui::SliderFloat4("", P, 0, 1, "%.3f", 1.0f);
		int hovered = ImGui::IsItemActive() || ImGui::IsItemHovered();
		ImGui::Dummy(ImVec2(0, 3));

		const float avail = ImGui::GetContentRegionAvailWidth();
		const float dim = ImMin(avail, 128.f);
		ImVec2 canvas(dim, dim);

		ImRect graphBox(window->DC.CursorPos, window->DC.CursorPos + canvas);
		ImGui::ItemSize(graphBox);

		if (!ImGui::ItemAdd(graphBox, NULL)) {
			return changed;
		}
		
		//const ImGuiID id = window->GetID(name.c_str());

		ImGui::RenderFrame(graphBox.Min, graphBox.Max, ImGui::GetColorU32(ImGuiCol_FrameBg, 1), true, style.FrameRounding);

		/*
		for (int i = 0; i <= canvas.x; i += (canvas.x / 4)) {
			drawList->AddLine(
				ImVec2(graphBox.Min.x + i, graphBox.Min.y),
				ImVec2(graphBox.Min.x + i, graphBox.Max.y),
				ImGui::GetColorU32(ImGuiCol_TextDisabled));
		}
		for (int i = 0; i <= canvas.y; i += (canvas.y / 4)) {
			drawList->AddLine(
				ImVec2(graphBox.Min.x, graphBox.Min.y + i),
				ImVec2(graphBox.Max.x, graphBox.Min.y + i),
				ImGui::GetColorU32(ImGuiCol_TextDisabled));
		}
		*/

		ImVec2 Q[4] = { { 0, 0 }, { P[0], P[1] }, { P[2], P[3] }, { 1, 1 } };
		ImVec2 results[segments + 1];
		bezierTable<segments>(Q, results);

		// control points: 2 lines and 2 circles
		const int circleRadius = 7;
		const int circleBorder = 3;
		const int lineWidth = 4;
		
			char buf[128];

			// handle gragraphBoxers
			for (int i = 0; i < 2; ++i) {
				ImVec2 pos = ImVec2(P[i * 2 + 0], 1 - P[i * 2 + 1]) * (graphBox.Max - graphBox.Min) + graphBox.Min;
				ImGui::SetCursorScreenPos(pos - ImVec2(circleRadius, circleRadius));
				ImGui::InvisibleButton((buf[0]++, buf), ImVec2(2 * circleRadius, 2 * circleRadius));
				if (ImGui::IsItemActive() || ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("(%4.3f, %4.3f)", P[i * 2 + 0], P[i * 2 + 1]);
				}
				if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
				{
					P[i * 2 + 0] += ImGui::GetIO().MouseDelta.x / canvas.x;
					P[i * 2 + 1] -= ImGui::GetIO().MouseDelta.y / canvas.y;
					changed = true;
				}
			}

			for (int i = 0; i < 4; i++) {
				if (P[i] < 0.f) {
					P[i] = 0.f;
				} else if (P[i] > 1.f) {
					P[i] = 1.f;
				}
			}

			if (hovered || changed) drawList->PushClipRectFullScreen();

			// draw curve
			ImColor color(ImGui::GetStyle().Colors[ImGuiCol_PlotLines]);
			for (int i = 0; i < segments; ++i) {
				ImVec2 p = { results[i + 0].x, 1 - results[i + 0].y };
				ImVec2 q = { results[i + 1].x, 1 - results[i + 1].y };
				ImVec2 r(p.x * (graphBox.Max.x - graphBox.Min.x) + graphBox.Min.x, p.y * (graphBox.Max.y - graphBox.Min.y) + graphBox.Min.y);
				ImVec2 s(q.x * (graphBox.Max.x - graphBox.Min.x) + graphBox.Min.x, q.y * (graphBox.Max.y - graphBox.Min.y) + graphBox.Min.y);
				drawList->AddLine(r, s, color, lineWidth);
			}

			// draw lines and gragraphBoxers
			float luma = ImGui::IsItemActive() || ImGui::IsItemHovered() ? 0.5f : 1.0f;
			ImVec4 pink(1.00f, 0.00f, 0.75f, luma), cyan(0.00f, 0.75f, 1.00f, luma);
			ImVec4 white(ImGui::GetStyle().Colors[ImGuiCol_Text]);
			ImVec2 p1 = ImVec2(P[0], 1 - P[1]) * (graphBox.Max - graphBox.Min) + graphBox.Min;
			ImVec2 p2 = ImVec2(P[2], 1 - P[3]) * (graphBox.Max - graphBox.Min) + graphBox.Min;
			drawList->AddLine(ImVec2(graphBox.Min.x, graphBox.Max.y), p1, ImColor(white), lineWidth);
			drawList->AddLine(ImVec2(graphBox.Max.x, graphBox.Min.y), p2, ImColor(white), lineWidth);
			drawList->AddCircleFilled(p1, circleRadius, ImColor(white));
			drawList->AddCircleFilled(p1, circleRadius - circleBorder, ImColor(pink));
			drawList->AddCircleFilled(p2, circleRadius, ImColor(white));
			drawList->AddCircleFilled(p2, circleRadius - circleBorder, ImColor(cyan));

			if (hovered || changed) drawList->PopClipRect();

			// restore cursor pos
			ImGui::SetCursorScreenPos(ImVec2(graphBox.Min.x, graphBox.Max.y + circleRadius)); // :P
		

		return changed;
	}
}