#include <M5Unified.h>

#include "utils.h"
#include "element.h"
#include "page.h"

namespace uilib {
	void Page::init(const point& size, color_t bg_color, LovyanGFX* gfx, int16_t color_depth){
		if(canvas){
			canvas->deleteSprite();
			delete canvas;
		}
		canvas = new M5Canvas(gfx);
		if(!canvas){
			UILIB_DEBUG_PRINTLN("Failed to create canvas");
			return;
		}
		canvas->setColorDepth(color_depth);
		canvas->createSprite(size.x, size.y);
		canvas->fillSprite(bg_color);
		canvas->setTextSize(1);
		canvas->pushSprite(0, 0);
		elements.clear();
		this->bg_color = bg_color;
		UILIB_DEBUG_PRINTLN("Page initialized with size: " + String(size.x) + "x" + String(size.y) + ", bg_color: " + String(bg_color) + ", color_depth: " + String(color_depth));
	}

	void Page::addElement(Element* element){
		elements.push_back(element);
	}

	void Page::setLoopFunction(std::function<void()> func){
		loop_function = func;
	}

	void Page::update(){
		loop();

		if(loop_function){
			loop_function();
		}

		if(!active){
			return;
		}

		if(!canvas){
			return;
		}
		bool flush = false;
		for(auto& element : elements){
			if (element->getParentElement() == nullptr) {
				flush |= element->update();
			}
		}
		if(flush || flush_required){
			canvas->fillSprite(bg_color);
			for(auto& element : elements){
				if(element->getParentElement() == nullptr && element->isVisible() && element != focused_element){
					element->draw();
				}
			}
			if(focused_element && focused_element->isVisible()){
				focused_element->draw();
			}
			canvas->pushSprite(0, 0);
			UILIB_DEBUG_PRINTLN("Page updated");
			flush_required = false;
		}
		return;
	}

	void Page::load(){
		active = true;
		if(!canvas){
			return;
		}
		for(auto& element : elements){
			if (element->getParentElement() == nullptr) {
				element->update(true);
			}
		}
		canvas->fillSprite(bg_color);
		for(auto& element : elements){
			if(element->getParentElement() == nullptr && element->isVisible() && element != focused_element){
				element->draw();
			}
		}
		if(focused_element && focused_element->isVisible()){
			focused_element->draw();
		}
		canvas->pushSprite(0, 0);
		
		flush_required = false;
		UILIB_DEBUG_PRINTLN("Page loaded");
		return;
	}

	void Page::setBackgroundColor(color_t color) {
		if (bg_color != color) {
			bg_color = color;
			flush_required = true;
			UILIB_DEBUG_PRINTLN("Page background color changed to: " + String(color));
		}
	}
}