#pragma once

#include <string>

#include "ImGuiNotify.hpp"
#include "imgui_internal.h"

enum class NotificationType
{
	None,
	Info,
	Success,
	Warning,
	Error
};

namespace ImGui
{
	static ImVec2 GetNextWindowSize()
	{
		// TODO: Maybe this doesnt work if we havent called it before hand....
		ImGuiContext& g = *GImGui;
		return g.NextWindowData.SizeVal;
	}
	
	static void BeginCenteredTitle(const char* title, ImGuiWindowFlags flags = 0)
	{
		ImVec2 titleSize = ImGui::CalcTextSize(title);

		float windowWidth = ImGui::GetMainViewport()->Size.x;
		float offset = (windowWidth - titleSize.x) * 0.5f;

		int spaces = int(offset / ImGui::CalcTextSize(" ").x);

		std::string centered = std::string(spaces, ' ') + title;

		ImGui::Begin(centered.c_str(), nullptr, flags);
	}
	
	static std::string GetCenteredWindowName(const char* name)
	{
		float windowWidth = GetNextWindowSize().x;
		
		float textWidth = ImGui::CalcTextSize(name).x;

		// Dont ask me why 0.625.. It works for the test usecase ok?
		float textStartX = (windowWidth - textWidth) * 0.625f;

		float styleOffset = ImGui::GetStyle().ItemInnerSpacing.x + ImGui::GetFontSize();
		textStartX -= styleOffset;
		
		if (textStartX < 0.0f) textStartX = 0.0f;

		float spaceWidth = ImGui::CalcTextSize(" ").x;
		int spaces = int(textStartX / spaceWidth);

		std::string centered = std::string(spaces, ' ') + name;
		return centered;
	}
	
	static bool CenteredSelectable(const char* label, bool selected, ImGuiSelectableFlags flags = 0, ImVec2 size = ImVec2(0,0))
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		// Measure text size
		ImVec2 textSize = ImGui::CalcTextSize(label);

		// Compute the full width of the selectable
		float selectableWidth = size.x > 0 ? size.x : ImGui::GetContentRegionAvail().x;
		float selectableHeight = size.y > 0 ? size.y : ImGui::GetContentRegionAvail().y;
		
		// Compute cursor X so that text is centered
		float cursorX = ImGui::GetCursorPosX() + (selectableWidth - textSize.x) * 0.5f;
		float cursorY = ImGui::GetCursorPosY() + (selectableHeight - textSize.y) * 0.5f;

		// Create the selectable
		bool pressed = ImGui::Selectable("##hidden", selected, flags, ImVec2(selectableWidth, size.y));

		// Draw text manually on top
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(cursorX, cursorY));
		ImGui::TextUnformatted(label);

		return pressed;
	}
	
	template<typename ...Args>
	void Notification(NotificationType aType, const std::string& aTitle, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToastType type = ImGuiToastType::None;
		switch (aType)
		{
		case NotificationType::None:
			type = ImGuiToastType::None;
			break;

		case NotificationType::Info:
			type = ImGuiToastType::Info;
			break;

		case NotificationType::Success:
			type = ImGuiToastType::Success;
			break;

		case NotificationType::Warning:
			type = ImGuiToastType::Warning;
			break;

		case NotificationType::Error:
			type = ImGuiToastType::Error;
			break;
		}

		ImGuiToast toast(type, 5000);
		toast.setTitle(aTitle.c_str());
		toast.setContent(aMessage, std::forward<Args>(someArgs)...);

		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void Notification(NotificationType aType, const std::string& aTitle, const int aDuration, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToastType type = ImGuiToastType::None;
		switch (aType)
		{
		case NotificationType::None:
			type = ImGuiToastType::None;
			break;

		case NotificationType::Info:
			type = ImGuiToastType::Info;
			break;

		case NotificationType::Success:
			type = ImGuiToastType::Success;
			break;

		case NotificationType::Warning:
			type = ImGuiToastType::Warning;
			break;

		case NotificationType::Error:
			type = ImGuiToastType::Error;
			break;
		}

		ImGuiToast toast(type, aDuration);
		toast.setTitle(aTitle.c_str());
		toast.setContent(aMessage, std::forward<Args>(someArgs)...);

		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void NotifyWarning(const std::string& aTitle, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToast toast(ImGuiToastType::Warning, 5000);
		toast.setTitle(aTitle.c_str());
		toast.setContent(aMessage, std::forward<Args>(someArgs)...);
		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void NotifyError(const std::string& aTitle, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToast toast(ImGuiToastType::Error, 5000);
		toast.setTitle(aTitle.c_str());
		toast.setContent(aMessage, std::forward<Args>(someArgs)...);
		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void NotifySuccess(const std::string& aTitle, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToast toast(ImGuiToastType::Success, 5000);
		toast.setTitle(aTitle.c_str());
		toast.setContent(aMessage, std::forward<Args>(someArgs)...);
		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void NotifyInfo(const std::string& aTitle, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToast toast(ImGuiToastType::Info, 50000);
		toast.setTitle(aTitle.c_str());
		toast.setContent(aMessage, std::forward<Args>(someArgs)...);
		ImGui::InsertNotification(toast);
	}
}