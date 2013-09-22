#include "BsPrerequisites.h"
#include "BsGUIWidget.h"
#include "CmInt2.h"
#include <boost/signal.hpp>

namespace BansheeEngine
{
	class BS_EXPORT GUIDropDownData
	{
		enum class Type
		{
			Separator,
			Entry,
			SubMenu
		};

	public:
		static GUIDropDownData separator();
		static GUIDropDownData button(const CM::WString& label, std::function<void()> callback);
		static GUIDropDownData subMenu(const CM::WString& label, const CM::Vector<GUIDropDownData>::type& entries);

		bool isSeparator() const { return mType == Type::Separator; }
		bool isSubMenu() const { return mType == Type::SubMenu; }

		const CM::WString& getLabel() const { return mLabel; }
		std::function<void()> getCallback() const { return mCallback; }
		const CM::Vector<GUIDropDownData>::type& getSubMenuEntries() const { return mChildEntries; }
	private:
		GUIDropDownData() { }

		std::function<void()> mCallback;
		CM::Vector<GUIDropDownData>::type mChildEntries;
		CM::WString mLabel;
		Type mType; 
	};

	/**
	 * @brief	Determines how will the drop down box be placed. Usually the system will attempt to position
	 * 			the drop box in a way so all elements can fit, and this class allows you to specify some limitations
	 * 			on how that works. 
	 * 			
	 * @note	For example, list boxes usually want drop down boxes to be placed above or below them, while
	 * 			context menus may want to have them placed around a single point in any direction.
	 */
	class BS_EXPORT GUIDropDownAreaPlacement
	{
	public:
		enum class Type
		{
			Position,
			BoundsVert,
			BoundsHorz
		};

		/**
		 * @brief	Drop down box will be placed at the specified position. By default the system
		 * 			prefers the top left corner of the box to correspond to the position, but if
		 * 			other corners offer more space for the contents, those will be used instead.
		 */
		static GUIDropDownAreaPlacement aroundPosition(const CM::Int2& position);

		/**
		 * @brief	Drop down box will be placed at the specified bounds. Box will be horizontally aligned to the left
		 * 			of the provided bounds. Vertically system prefers placing the box at the bottom of the bounds, but may choose
		 * 			to align it with the top of the bounds if it offers more space for the contents.
		 */
		static GUIDropDownAreaPlacement aroundBoundsVert(const CM::Rect& bounds);
		
		/**
		 * @brief	Drop down box will be placed at the specified bounds. Box will be vertically aligned to the top
		 * 			of the provided bounds. Horizontally system prefers placing the box at the right of the bounds, but may choose
		 * 			to align it with the left of the bounds if it offers more space for the contents.
		 */
		static GUIDropDownAreaPlacement aroundBoundsHorz(const CM::Rect& bounds);

		Type getType() const { return mType; }
		const CM::Rect& getBounds() const { return mBounds; }
		const CM::Int2& getPosition() const { return mPosition; }

	private:
		GUIDropDownAreaPlacement() { }

		Type mType;
		CM::Rect mBounds;
		CM::Int2 mPosition;
	};

	enum class GUIDropDownType
	{
		ListBox,
		ContextMenu,
		MenuBar
	};

	/**
	 * @brief	This is a generic GUI drop down box class that can be used for:
	 * 			list boxes, menu bars or context menus.
	 */
	class BS_EXPORT GUIDropDownBox : public GUIWidget
	{
	public:
		GUIDropDownBox(const CM::HSceneObject& parent);
		~GUIDropDownBox();

		void initialize(CM::Viewport* target, CM::RenderWindow* window, const GUIDropDownAreaPlacement& placement,
			const CM::Vector<GUIDropDownData>::type& elements, const GUISkin& skin, GUIDropDownType type);
	private:
		static const CM::UINT32 DROP_DOWN_BOX_WIDTH;

		GUIDropDownType mType;
		CM::Vector<GUIDropDownData>::type mElements;
		CM::UINT32 mPage;
		CM::INT32 x, y;
		CM::UINT32 width, height;

		CM::Vector<GUITexture*>::type mCachedSeparators;
		CM::Vector<GUIButton*>::type mCachedEntryBtns;
		CM::Vector<GUIButton*>::type mCachedExpEntryBtns;
		GUIButton* mScrollUpBtn;
		GUIButton* mScrollDownBtn;
		GUITexture* mBackgroundFrame;

		const GUIElementStyle* mScrollUpStyle;
		const GUIElementStyle* mScrollDownStyle;
		const GUIElementStyle* mEntryBtnStyle;
		const GUIElementStyle* mEntryExpBtnStyle;
		const GUIElementStyle* mSeparatorStyle;
		const GUIElementStyle* mBackgroundStyle;
		SpriteTexturePtr mScrollUpBtnArrow;
		SpriteTexturePtr mScrollDownBtnArrow;

		GUIArea* mBackgroundArea;
		GUIArea* mContentArea;
		GUILayout* mContentLayout;

		CM::HSceneObject mSubMenuSO;
		CM::GameObjectHandle<GUIDropDownBox> mSubMenuDropDownBox;

		void updateGUIElements();

		void scrollDown();
		void scrollUp();

		CM::UINT32 getElementHeight(CM::UINT32 idx) const;

		void elementClicked(CM::UINT32 idx);
		void openSubMenu(GUIButton* source, CM::UINT32 elementIdx);
		void closeSubMenu();
	};
}