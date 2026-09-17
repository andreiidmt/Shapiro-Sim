#pragma once
#include <glad/glad.h>         
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Objects.h"
#include "Computations.h"

#include <limits>

class Dashboard {
public:
	StartingPoint& startPos;
	IntermediaryObject& interMass;
	DestinationPoint& destinationPos;

	Dashboard(StartingPoint& sp, IntermediaryObject& im, DestinationPoint& dp) : startPos(sp), interMass(im), destinationPos(dp) {}

	void stylize() {
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowRounding = 0.0f;
		style.FrameRounding = 6.0f;
		style.ScrollbarRounding = 6.0f;
		style.WindowPadding = ImVec2(16.0f, 16.0f);
		style.FramePadding = ImVec2(10.0f, 8.0f);
		style.ItemSpacing = ImVec2(10.0f, 10.0f);

		// Slate / Modern Dark Palette
		ImVec4* colors = style.Colors;
		colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.08f, 0.10f, 1.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.14f, 0.18f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.19f, 0.24f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.24f, 0.30f, 1.00f);

		// Text & Borders
		colors[ImGuiCol_Text] = ImVec4(0.92f, 0.94f, 0.96f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.53f, 0.60f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.15f, 0.17f, 0.22f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	}

	void init(GLFWwindow* window) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImFont* font = io.Fonts->AddFontFromFileTTF("assets/fonts/montserrat/static/Montserrat-Black.ttf", 20.0f);
		ImGui::StyleColorsDark();
		this->stylize();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void renderDashboard(int windowWidth, int windowHeight) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2((float)windowWidth, (float)windowHeight));


		ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

		ImGui::Begin("Dashboard", NULL, flags);

		ImGui::Text("Starting point coordinates(AU).");
		ImGui::Separator();
		double startx = startPos.getX(), starty = startPos.getY(), startz = startPos.getZ();
		if( ImGui::InputDouble("X##start", &startx))
			startPos.setX(std::clamp(startx,-100.0,100.0));
		if (ImGui::InputDouble("Y##start", &starty))
			startPos.setY(std::clamp(starty,-100.0,100.0));
		if (ImGui::InputDouble("Z##start", &startz))
			startPos.setZ(std::clamp(startz,-100.0,100.0));
		
		ImGui::Text("Destination point coordinates(AU).");
		ImGui::Separator();
		double destx = destinationPos.getX(), desty = destinationPos.getY(), destz = destinationPos.getZ();
		if (ImGui::InputDouble("X##destination", &destx))
			destinationPos.setX(std::clamp(destx,-100.0,100.0));
		if (ImGui::InputDouble("Y##destination", &desty))
			destinationPos.setY(std::clamp(desty,-100.0,100.0));
		if (ImGui::InputDouble("Z##destination", &destz))
			destinationPos.setZ(std::clamp(destz,-100.0,100.0));
		
		ImGui::Text("Intermediary object data.");
		ImGui::Separator();
		double interomass = interMass.getMass(), interoradius = interMass.getRadius();
		if (ImGui::InputDouble("Mass(kg).", &interomass, 0.0, 0.0, "%.4e"))
			interMass.setMass(std::clamp(interomass,0.0,1e43));
		if (ImGui::InputDouble("Radius(m)", &interoradius, 0.0, 0.0, "%.4e"))
			interMass.setRadius(interoradius);
		double rs = schwarzschildRadius(interMass);
		ImGui::Text("Schwarzschild radius: %.4e m.", rs);
		double cip = criticalImpactParameter(interMass);
		ImGui::PushTextWrapPos(0.0f);
		ImGui::Text("Critical impact parameter(light can't escape): %.4e m", cip);
		ImGui::PopTextWrapPos();

		ImGui::PushTextWrapPos(0.0f);
		ImGui::Spacing();

		double ut = unperturbedTime(startPos, destinationPos);
		double dt = shapiroTimeDelayFixed(startPos, interMass, destinationPos);
		double tt = ut + dt;
		double dta = shapiroTimeDelayApproximation(startPos, interMass, destinationPos);
		double dtaError = approximationError(dt, dta);
		double ip = impactParameterComputation(startPos, destinationPos);

		if (interMass.getRadius() <= rs && ip <= cip) {
			dt = std::numeric_limits<double>::infinity();
			tt = std::numeric_limits<double>::infinity();
			dta = std::numeric_limits<double>::infinity();
			dtaError = std::numeric_limits<double>::infinity();
		}

		ImGui::Text("Analytical results.");
		ImGui::Separator();

		ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersInnerH |
			ImGuiTableFlags_RowBg |
			ImGuiTableFlags_PadOuterX;

		if (ImGui::BeginTable("ResultsTable", 2, tableFlags)) {
			ImGui::TableSetupColumn("Metric", ImGuiTableColumnFlags_WidthFixed, 180.0f);
			ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableHeadersRow();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Unperturbed Time");
			ImGui::TableNextColumn();
			ImGui::Text("%.2e s (%.2f min)", ut, ut / 60.0);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Time Delay");
			ImGui::TableNextColumn();
			ImGui::Text("%.6f µs", dt * 1e6);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Total Time");
			ImGui::TableNextColumn();
			ImGui::Text("%.2e s", tt);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Time delay approximation");
			ImGui::TableNextColumn();
			ImGui::Text("%.6f µs", dta * 1e6);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Approximation error");
			ImGui::TableNextColumn();
			ImGui::Text("%.2f%%", dtaError);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Impact parameter(light's unperturbed closest approach");
			ImGui::TableNextColumn();
			ImGui::Text("%.2e m", ip);

			ImGui::EndTable();
		}

		

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Text("NOTES: 1. The dashed line represent light's unperturbed time.(rendering the actual bending of the ray is not produced).");
		ImGui::Text("2. In the case of a black hole, if the impact parameter is smaller or equal to the critical impact parameter, the light will be captured.");

		ImGui::PopTextWrapPos();
		ImGui::End();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void cleanup() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
};