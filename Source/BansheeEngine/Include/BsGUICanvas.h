//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsPrerequisites.h"
#include "BsGUIElement.h"
#include "BsImageSprite.h"
#include "BsTextSprite.h"

namespace BansheeEngine
{
	/** @addtogroup GUI
	 *  @{
	 */

	/**	
	 * A GUI element that allows the user to draw custom graphics. All drawn elements relative to the canvas, to its origin
	 * in the top left corner.
	 */
	class BS_EXPORT GUICanvas : public GUIElement
	{
	public:
		/** Returns type name of the GUI element used for finding GUI element styles.  */
		static const String& getGUITypeName();

		/**
		 * Creates a new GUI canvas element.
		 *
		 * @param[in]	options			Options that allow you to control how is the element positioned and sized. This will
		 *								override any similar options set by style.
		 * @param[in]	styleName		Optional style to use for the element. Style will be retrieved from GUISkin of the
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		static GUICanvas* create(const GUIOptions& options, const String& styleName = StringUtil::BLANK);

		/**
		 * Creates a new GUI canvas element.
		 *
		 * @param[in]	styleName		Optional style to use for the element. Style will be retrieved from GUISkin of the 
		 *								GUIWidget the element is used on. If not specified default style is used.
		 */
		static GUICanvas* create(const String& styleName = StringUtil::BLANK);
		
		/** 
		 * Draws a line going from @p a to @p b.
		 *
		 * @param[in]	a		Starting point of the line, relative to the canvas origin (top-left).
		 * @param[in]	b		Ending point of the line, relative to the canvas origin (top-left).
		 * @param[in]	color	Color of the line.
		 */
		void drawLine(const Vector2I& a, const Vector2I& b, const Color& color = Color::White);

		/** 
		 * Draws multiple lines following the path by the provided vertices. First vertex connects to the second vertex,
		 * and every following vertex connects to the previous vertex.
		 *
		 * @param[in]	vertices	Points to use for drawing the line. Must have at least two elements. All points are 
		 *							relative to the canvas origin (top-left).
		 * @param[in]	color		Color of the line.
		 */
		void drawPolyLine(const Vector<Vector2I>& vertices, const Color& color = Color::White);

		/** 
		 * Draws a quad with a the provided texture displayed.
		 *
		 * @param[in]	texture		Texture to draw.
		 * @param[in]	area		Position and size of the texture to draw. Position is relative to the canvas origin 
		 *							(top-left). If size is zero, the default texture size will be used.
		 * @param[in]	scaleMode	Scale mode to use when sizing the texture. Only relevant if the provided quad size
		 *							doesn't match the texture size.
		 * @param[in]	color		Color to tint the drawn texture with.
		 */
		void drawTexture(const HSpriteTexture& texture, const Rect2I& area, 
			TextureScaleMode scaleMode = TextureScaleMode::StretchToFit, const Color& color = Color::White);

		/** 
		 * Draws a triangle strip. First three vertices are used to form the initial triangle, and every next vertex will
		 * form a triangle with the previous two.
		 *
		 * @param[in]	vertices	A set of points defining the triangles. Must have at least three elements. All points
		 *							are relative to the canvas origin (top-left).
		 * @param[in]	color		Color of the triangles.
		 */
		void drawTriangleStrip(const Vector<Vector2I>& vertices, const Color& color = Color::White);

		/** 
		 * Draws a triangle list. Every three vertices in the list represent a unique triangle.
		 *
		 * @param[in]	vertices	A set of points defining the triangles. Must have at least three elements, and its size
		 *							must be a multiple of three.
		 * @param[in]	color		Color of the triangles.
		 */
		void drawTriangleList(const Vector<Vector2I>& vertices, const Color& color = Color::White);

		/**
		 * Draws a piece of text with the wanted font. The text will be aligned to the top-left corner of the provided
		 * position, and will not be word wrapped.
		 *
		 * @param[in]	text		Text to draw.
		 * @param[in]	position	Position of the text to draw. This represents the top-left corner of the text. It is
		 *							relative to the canvas origin (top-left).
		 * @param[in]	size		Size of the font.
		 * @param[in]	color		Color of the text.
		 */
		void drawText(const WString& text, const Vector2I& position, const HFont& font, UINT32 size = 10, 
			const Color& color = Color::White);

		/** Clears the canvas, removing any previously drawn elements. */
		void clear();

	public: // ***** INTERNAL ******
		/** @name Internal
		 *  @{
		 */

		/** @copydoc GUIElement::_getOptimalSize */
		Vector2I _getOptimalSize() const override;

		/** @} */
	protected:
		/** Type of elements that may be drawn on the canvas. */
		enum class CanvasElementType
		{
			Line,
			Triangle,
			Image,
			Text
		};

		/** Represents a single element drawn by the canvas. */
		struct CanvasElement
		{
			CanvasElementType type;
			Color color;

			union
			{
				ImageSprite* imageSprite;
				UINT32 imageDataId;
				TextureScaleMode scaleMode;
			};

			union
			{
				UINT32 vertexStart;
				UINT32 numVertices;
			};

			union
			{
				TextSprite* textSprite;
				UINT32 textDataId;
				UINT32 size;
			};
		};

		/** Information required for drawing a text canvas element. */
		struct TextElementData
		{
			WString string;
			HFont font;
			Vector2I position;
		};

		/** Information required for drawing an image canvas element. */
		struct ImageElementData
		{
			HSpriteTexture texture;
			Rect2I area;
		};

		GUICanvas(const String& styleName, const GUIDimensions& dimensions);
		virtual ~GUICanvas();

		/** @copydoc GUIElement::_getNumRenderElements */
		UINT32 _getNumRenderElements() const override;

		/** @copydoc GUIElement::_getMaterial */
		const SpriteMaterialInfo& _getMaterial(UINT32 renderElementIdx) const override;

		/** @copydoc GUIElement::_getNumQuads */
		UINT32 _getNumQuads(UINT32 renderElementIdx) const override;

		/** @copydoc GUIElement::_fillBuffer */
		void _fillBuffer(UINT8* vertices, UINT8* uv, UINT32* indices, UINT32 startingQuad, 
			UINT32 maxNumQuads, UINT32 vertexStride, UINT32 indexStride, UINT32 renderElementIdx) const override;

		/** @copydoc GUIElement::updateRenderElementsInternal */
		void updateRenderElementsInternal() override;

		/** Build an image sprite from the provided canvas element. */
		void buildImageElement(const CanvasElement& element);

		/** Build a text sprite from the provided canvas element. */
		void buildTextElement(const CanvasElement& element);

		Vector<CanvasElement> mElements;

		Vector<ImageElementData> mImageData;
		Vector<TextElementData> mTextData;
		Vector<Vector2I> mVertexData;

		Vector2I* mOutVertices;
	};

	/** @} */
}