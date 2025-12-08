#pragma once

#include <string>

#include "imgui_internal.h"
#include "imgui_notify.h"

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
	static void BeginCenteredTitle(const char* title, ImGuiWindowFlags flags = 0)
	{
		// Compute the centered title by prepending spaces
		// until the rendered text width is visually centered.
		ImVec2 titleSize = ImGui::CalcTextSize(title);

		float windowWidth = ImGui::GetMainViewport()->Size.x; // or use your own width
		float offset = (windowWidth - titleSize.x) * 0.5f;

		// Convert offset to number of spaces (approximate)
		int spaces = int(offset / ImGui::CalcTextSize(" ").x);

		// Build a buffer with the required spaces
		std::string centered = std::string(spaces, ' ') + title;

		ImGui::Begin(centered.c_str(), nullptr, flags);
	}
	
	static bool BeginCenteredTitlePopupModal(const char* name, bool* p_open, ImGuiWindowFlags flags = 0)
	{
		// Compute the centered title by prepending spaces
		// until the rendered text width is visually centered.
		ImVec2 titleSize = ImGui::CalcTextSize(name);

		float windowWidth = ImGui::GetMainViewport()->Size.x; // or use your own width
		float offset = (windowWidth - titleSize.x) * 0.5f;

		// Convert offset to number of spaces (approximate)
		int spaces = int(offset / ImGui::CalcTextSize(" ").x);

		// Build a buffer with the required spaces
		std::string centered = std::string(spaces, ' ') + name;

		return ImGui::BeginPopupModal(centered.c_str(), p_open, flags);
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
		ImGuiToastType type = ImGuiToastType_None;
		switch (aType)
		{
		case NotificationType::None:
			type = ImGuiToastType_None;
			break;

		case NotificationType::Info:
			type = ImGuiToastType_Info;
			break;

		case NotificationType::Success:
			type = ImGuiToastType_Success;
			break;

		case NotificationType::Warning:
			type = ImGuiToastType_Warning;
			break;

		case NotificationType::Error:
			type = ImGuiToastType_Error;
			break;
		}

		ImGuiToast toast(type, 5000);
		toast.set_title(aTitle.c_str());
		toast.set_content(aMessage, std::forward<Args>(someArgs)...);

		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void Notification(NotificationType aType, const std::string& aTitle, const int aDuration, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToastType type = ImGuiToastType_None;
		switch (aType)
		{
		case NotificationType::None:
			type = ImGuiToastType_None;
			break;

		case NotificationType::Info:
			type = ImGuiToastType_Info;
			break;

		case NotificationType::Success:
			type = ImGuiToastType_Success;
			break;

		case NotificationType::Warning:
			type = ImGuiToastType_Warning;
			break;

		case NotificationType::Error:
			type = ImGuiToastType_Error;
			break;
		}

		ImGuiToast toast(type, aDuration);
		toast.set_title(aTitle.c_str());
		toast.set_content(aMessage, std::forward<Args>(someArgs)...);

		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void NotifyWarning(const std::string& aTitle, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToast toast(ImGuiToastType_Warning, 5000);
		toast.set_title(aTitle.c_str());
		toast.set_content(aMessage, std::forward<Args>(someArgs)...);
		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void NotifyError(const std::string& aTitle, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToast toast(ImGuiToastType_Error, 5000);
		toast.set_title(aTitle.c_str());
		toast.set_content(aMessage, std::forward<Args>(someArgs)...);
		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void NotifySuccess(const std::string& aTitle, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToast toast(ImGuiToastType_Success, 5000);
		toast.set_title(aTitle.c_str());
		toast.set_content(aMessage, std::forward<Args>(someArgs)...);
		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void NotifyInfo(const std::string& aTitle, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToast toast(ImGuiToastType_Info, 5000);
		toast.set_title(aTitle.c_str());
		toast.set_content(aMessage, std::forward<Args>(someArgs)...);
		ImGui::InsertNotification(toast);
	}
}