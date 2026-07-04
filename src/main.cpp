#include <M5Unified.h>
#include <M5GFX.h>
#include <map>
#include <M5UILib.h>

M5GFX display;

namespace PageManager = uilib::PageManager;

class PageHome : public uilib::Page
{
private:
	uilib::Label label_title;
	uilib::Grid grid;

	uilib::DButton btn1, btn2, btn3;
	uilib::DButton btn4, btn5, btn6, btn7, btn8, btn9;

public:
	void loop() override
	{
		if (btn1.wasReleased())
		{
			PageManager::set(1);
		}
		if (btn2.wasReleased())
		{
			PageManager::set(2);
		}
		if (btn3.wasReleased())
		{
			PageManager::set(3);
		}
		if (btn4.wasReleased())
		{
			PageManager::set(4);
		}
		if (btn5.wasReleased())
		{
			PageManager::set(5);
		}
		if (btn6.wasReleased())
		{
			PageManager::set(6);
		}
		if (btn7.wasReleased())
		{
			PageManager::set(7);
		}
		if (btn8.wasReleased())
		{
			PageManager::set(8);
		}
		if (btn9.wasReleased())
		{
			PageManager::set(9);
		}
	}

	void init() override
	{
		uilib::Page::init({M5.Lcd.width(), M5.Lcd.height()}, BLACK, &M5.Lcd, 16);

		// タイトルラベル
		uilib::Color title_fg(255, 255, 255);
		uilib::Color title_bg = uilib::Color::fromHSV(0.0f, 1.0f, 0.4f); // 赤系のタイトル背景
		label_title = uilib::Label("M5 UILib Home", {160, 15}, title_fg, title_bg, 1.0f, this, &fonts::lgfxJapanGothic_20, 4, 4);
		label_title.setAlign(uilib::Align::CENTER);

		// 3行3列 of Grid (完全に埋める)
		grid = uilib::Grid({20, 60}, 280, 150, 3, 3, 0x18C3, this);
		grid.setStrokeWidth(2);
		grid.setBorderColor(uilib::Color(100, 255, 100));
		grid.setPadding(8, 8, 8, 8);
		grid.setSpacing(10, 10);

		// ボタン初期化
		uilib::Color btn_bg(50, 50, 150);
		btn1 = uilib::DButton("Demo 1", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 4, 6);
		btn2 = uilib::DButton("Demo 2", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 4, 6);
		btn3 = uilib::DButton("Demo 3", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 4, 6);
		btn4 = uilib::DButton("Demo 4", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 4, 6);
		btn5 = uilib::DButton("Demo 5", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 4, 6);
		btn6 = uilib::DButton("Demo 6", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 4, 6);
		btn7 = uilib::DButton("Demo 7", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 4, 6);
		btn8 = uilib::DButton("Demo 8", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 4, 6);
		btn9 = uilib::DButton("Keyboard", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 4, 6);

		grid.addChild(&btn1);
		grid.addChild(&btn2);
		grid.addChild(&btn3);
		grid.addChild(&btn4);
		grid.addChild(&btn5);
		grid.addChild(&btn6);
		grid.addChild(&btn7);
		grid.addChild(&btn8);
		grid.addChild(&btn9);

		// ページに要素を登録
		addElement(&label_title);
		addElement(&grid);

		addElement(&btn1);
		addElement(&btn2);
		addElement(&btn3);
		addElement(&btn4);
		addElement(&btn5);
		addElement(&btn6);
		addElement(&btn7);
		addElement(&btn8);
		addElement(&btn9);

		PageManager::add(this, 0);
	}
};

class Page1 : public uilib::Page
{
private:
	uilib::Label label1;
	uilib::Label label2;
	uilib::Label label3;

	uilib::DButton button1;
	uilib::DButton button2;
	uilib::DButton button3;

	int i = 0;

public:
	void loop() override
	{
		if (M5.BtnA.wasPressed())
		{
			i++;				   // Increment the weight value when BtnA is pressed
			label3.set(String(i)); // Update the label text to show the current weight value
		}
		if (M5.BtnC.wasPressed())
		{
			i--;				   // Decrement the weight value when BtnC is pressed
			label3.set(String(i)); // Update the label text to show the current weight value
		}

		// Demonstration of the new DButton functions
		if (button1.wasReleased())
		{
			Serial.printf("Button released! Held for: %d ms (counts: %d)\n", button1.getPressDurationMs(), button1.getPressCount());
			i += 10; // Add 10 when the virtual button is pressed and released
			label3.set(String(i));
			setBackgroundColor(0x03E0); // Test: Change background color to dark green
		}
		if (button1.isPressed())
		{
			static uint32_t last_print = 0;
			if (millis() - last_print > 500)
			{
				Serial.printf("Button pressing... current duration: %d ms, counts: %d\n", button1.getPressDurationMs(), button1.getPressCount());
				last_print = millis();
			}
		}
		if (button2.wasReleased())
		{
			Serial.printf("Button released! Held for: %d ms (counts: %d)\n", button2.getPressDurationMs(), button2.getPressCount());
			i -= 10; // Add 10 when the virtual button is pressed and released
			label3.set(String(i));
			setBackgroundColor(BLACK); // Test: Reset background color to black
		}
		if (button2.isPressed())
		{
			static uint32_t last_print = 0;
			if (millis() - last_print > 500)
			{
				Serial.printf("Button pressing... current duration: %d ms, counts: %d\n", button2.getPressDurationMs(), button2.getPressCount());
				last_print = millis();
			}
		}
		if (button3.wasReleased())
		{
			Serial.printf("Button released! Held for: %d ms (counts: %d)\n", button3.getPressDurationMs(), button3.getPressCount());
			PageManager::set(2);
			// label3.set(String(i));
		}
		if (button3.isPressed())
		{
			static uint32_t last_print = 0;
			if (millis() - last_print > 500)
			{
				Serial.printf("Button pressing... current duration: %d ms, counts: %d\n", button3.getPressDurationMs(), button3.getPressCount());
				last_print = millis();
			}
		}
	}

	void init() override
	{
		uilib::Page::init({M5.Lcd.width(), M5.Lcd.height()}, BLACK, &M5.Lcd, 16);
		label1 = uilib::Label("TESTてすとテストABC123試験", {15, 15}, GREEN, NAVY, 1.0f, this, &fonts::lgfxJapanGothic_20, 4, 4);
		label2 = uilib::Label("WEIGHT:", {15, 55}, WHITE, BLACK, 1.0f, this, &fonts::efontJA_16);
		label3 = uilib::Label("0", {95, 55}, GREEN, BLACK, 1.0f, this, &fonts::efontJA_16);

		button1 = uilib::DButton("Add", {15, 105}, RED, GREEN, 1.0f, this, &fonts::lgfxJapanGothic_20, 5, 8);
		button2 = uilib::DButton("Dec", {80, 105}, WHITE, PURPLE, 1.0f, this, &fonts::lgfxJapanGothic_20, 5, 8);
		button3 = uilib::DButton("Change", {220, 195}, BLUE, WHITE, 1.0f, this, &fonts::lgfxJapanGothic_20, 10, 4);
		addElement(&button1);
		addElement(&button2);
		addElement(&button3);

		addElement(&label1);
		addElement(&label2);
		addElement(&label3);

		PageManager::add(this, 1);
	}
};

class Page2 : public uilib::Page
{
private:
	uilib::Label label1;
	uilib::Label label2;
	uilib::Label label3;

	uilib::DButton button1;
	uilib::DButton button2;
	uilib::DButton button3;
	uilib::ToggleSwitch toggle1;
	uilib::Slider slider1;
	int i = 0;

public:
	void loop() override
	{
		if (M5.BtnA.wasPressed())
		{
			i++; // Increment the weight value when BtnA is pressed
			if (i > 100)
				i = 100;
			label3.set(String(i)); // Update the label text to show the current weight value
			slider1.setValue(i);
		}
		if (M5.BtnC.wasPressed())
		{
			i--; // Decrement the weight value when BtnC is pressed
			if (i < 0)
				i = 0;
			label3.set(String(i)); // Update the label text to show the current weight value
			slider1.setValue(i);
		}

		// Demonstration of the new DButton functions
		if (button1.wasReleased())
		{
			Serial.printf("Button released! Held for: %d ms (counts: %d)\n", button1.getPressDurationMs(), button1.getPressCount());
			i += 10; // Add 10 when the virtual button is pressed and released
			if (i > 100)
				i = 100;
			label3.set(String(i));
			slider1.setValue(i);
		}
		if (button1.isPressed())
		{
			static uint32_t last_print = 0;
			if (millis() - last_print > 500)
			{
				Serial.printf("Button pressing... current duration: %d ms, counts: %d\n", button1.getPressDurationMs(), button1.getPressCount());
				last_print = millis();
			}
		}
		if (button2.wasReleased())
		{
			Serial.printf("Button released! Held for: %d ms (counts: %d)\n", button2.getPressDurationMs(), button2.getPressCount());
			i -= 10; // Add 10 when the virtual button is pressed and released
			if (i < 0)
				i = 0;
			label3.set(String(i));
			slider1.setValue(i);
		}
		if (button2.isPressed())
		{
			static uint32_t last_print = 0;
			if (millis() - last_print > 500)
			{
				Serial.printf("Button pressing... current duration: %d ms, counts: %d\n", button2.getPressDurationMs(), button2.getPressCount());
				last_print = millis();
			}
		}
		if (button3.wasReleased())
		{
			Serial.printf("Button released! Held for: %d ms (counts: %d)\n", button3.getPressDurationMs(), button3.getPressCount());
			PageManager::set(3);
			label3.set(String(i));
		}
		if (button3.isPressed())
		{
			static uint32_t last_print = 0;
			if (millis() - last_print > 500)
			{
				Serial.printf("Button pressing... current duration: %d ms, counts: %d\n", button3.getPressDurationMs(), button3.getPressCount());
				last_print = millis();
			}
		}
	}

	void init() override
	{
		uilib::Page::init({M5.Lcd.width(), M5.Lcd.height()}, BLACK, &M5.Lcd, 16);
		label1 = uilib::Label("Page2TEST", {15, 15}, GREEN, NAVY, 1.0f, this, &fonts::lgfxJapanGothic_20, 4, 4);
		label2 = uilib::Label("WEIGHT:", {15, 55}, WHITE, BLACK, 1.0f, this, &fonts::efontJA_16);
		label3 = uilib::Label("0", {95, 55}, GREEN, BLACK, 1.0f, this, &fonts::efontJA_16);

		button1 = uilib::DButton("Add", {15, 105}, RED, GREEN, 1.0f, this, &fonts::lgfxJapanGothic_20, 5, 8);
		button2 = uilib::DButton("Dec", {80, 105}, WHITE, PURPLE, 1.0f, this, &fonts::lgfxJapanGothic_20, 5, 8);
		button3 = uilib::DButton("Change", {220, 195}, BLUE, WHITE, 1.0f, this, &fonts::lgfxJapanGothic_20, 10, 4);

		toggle1 = uilib::ToggleSwitch({150, 105}, {60, 30}, false, 0x07E0, 0x7BEF, 0xFFFF, this);
		toggle1.setOnToggleCallback([this](bool state)
									{
			Serial.printf("Toggle switch state changed: %s\n", state ? "ON" : "OFF");
			button1.setEnabled(!state);
			button2.setEnabled(!state);
			slider1.setEnabled(!state); });

		slider1 = uilib::Slider({150, 145}, {150, 20}, 0.0f, 100.0f, 1.0f, 0.0f, GREEN, this);
		slider1.setOnValueChangeCallback([this](float val)
										 {
			Serial.printf("Slider value changed: %.1f\n", val);
			i = (int)val;
			label3.set(String(i)); });

		addElement(&button1);
		addElement(&button2);
		addElement(&button3);
		addElement(&toggle1);
		addElement(&slider1);

		addElement(&label1);
		addElement(&label2);
		addElement(&label3);

		PageManager::add(this, 2);
	}
};

class Page3 : public uilib::Page
{
private:
	uilib::Label label1;
	uilib::Checkbox checkbox1;
	uilib::RadioButtonGroup group1;
	uilib::RadioButton radio1;
	uilib::RadioButton radio2;
	uilib::RadioButton radio3;
	uilib::ToggleSwitch toggle1;
	uilib::DButton button3;
	uilib::Dropdown dropdown1;
	uilib::Dropdown dropdown2;

public:
	void loop() override
	{
		if (button3.wasReleased())
		{
			Serial.println("Next button released! Moving to Page 4");
			PageManager::set(4);
		}
	}

	void init() override
	{
		uilib::Page::init({M5.Lcd.width(), M5.Lcd.height()}, BLACK, &M5.Lcd, 16);

		// 新しいColorクラスを使って色を指定
		// 文字色: RGB指定 (明るい緑), 背景色: HSV指定 (濃い青)
		uilib::Color title_fg(0, 255, 100);
		uilib::Color title_bg = uilib::Color::fromHSV(240.0f, 1.0f, 0.5f);
		label1 = uilib::Label("Page3TEST", {160, 15}, title_fg, title_bg, 1.0f, this, &fonts::lgfxJapanGothic_20, 4, 4);
		label1.setAlign(uilib::Align::CENTER);

		checkbox1 = uilib::Checkbox("Check Me!", {15, 55}, false, WHITE, BLACK, 1.0f, this, &fonts::efontJA_16);
		checkbox1.setOnChangeCallback([](bool checked)
									  { Serial.printf("Checkbox state changed: %s\n", checked ? "Checked" : "Unchecked"); });

		group1.setOnChangeCallback([](int val)
								   { Serial.printf("Radio group selected value: %d\n", val); });

		radio1 = uilib::RadioButton("Option A", {15, 95}, 1, WHITE, BLACK, 1.0f, this, &fonts::efontJA_16);
		radio2 = uilib::RadioButton("Option B", {15, 125}, 2, WHITE, BLACK, 1.0f, this, &fonts::efontJA_16);
		radio3 = uilib::RadioButton("Option C", {15, 155}, 3, WHITE, BLACK, 1.0f, this, &fonts::efontJA_16);

		group1.addButton(&radio1);
		group1.addButton(&radio2);
		group1.addButton(&radio3);

		std::vector<String> dp_items = {"Apple", "Banana", "Cherry", "Date", "Elderberry", "Fig", "Grape"};
		// 文字色: RGBで柔らかいオレンジ色を指定
		uilib::Color dp_border(255, 150, 50);
		dropdown1 = uilib::Dropdown(dp_items, {230, 95}, 155, 30, 0, dp_border, BLACK, 1.0f, this, &fonts::efontJA_16);
		dropdown1.setAlign(uilib::Align::CENTER);
		dropdown1.setOnChangeCallback([](int idx)
									  { Serial.printf("Dropdown 1 selected: %d\n", idx); });

		std::vector<String> dp2_items = {"Item 1", "Item 2", "Item 3"};
		// 文字色: 8bit整数HSVで生成した色を指定
		uilib::Color dp2_border = uilib::Color::fromHSV(static_cast<uint8_t>(160), static_cast<uint8_t>(255), static_cast<uint8_t>(255));
		dropdown2 = uilib::Dropdown(dp2_items, {305, 145}, 155, 30, -1, dp2_border, BLACK, 1.0f, this, &fonts::efontJA_16);
		dropdown2.setAlign(uilib::Align::RIGHT);
		dropdown2.setOnChangeCallback([](int idx)
									  { Serial.printf("Dropdown 2 selected: %d\n", idx); });

		toggle1 = uilib::ToggleSwitch({180, 55}, {60, 30}, false, 0x07E0, 0x7BEF, 0xFFFF, this);
		toggle1.setOnToggleCallback([this](bool state)
									{
			Serial.printf("Toggle switch state changed: %s\n", state ? "ON" : "OFF");
			checkbox1.setEnabled(!state);
			radio1.setEnabled(!state);
			radio2.setEnabled(!state);
			radio3.setEnabled(!state);
			dropdown1.setEnabled(!state);
			dropdown2.setEnabled(!state); });

		button3 = uilib::DButton("Change", {220, 195}, BLUE, WHITE, 1.0f, this, &fonts::lgfxJapanGothic_20, 10, 4);

		addElement(&label1);
		addElement(&checkbox1);
		addElement(&radio1);
		addElement(&radio2);
		addElement(&radio3);
		addElement(&dropdown1);
		addElement(&dropdown2);
		addElement(&toggle1);
		addElement(&button3);

		PageManager::add(this, 3);
	}
};

class Page4 : public uilib::Page
{
private:
	uilib::Label label1;
	uilib::DButton button3;
	uilib::Shape shape1;
	uilib::Shape shape2;
	uilib::Shape shape3;
	float angle = 0.0f;

public:
	void loop() override
	{
		if (button3.wasReleased())
		{
			Serial.println("Next button released! Moving to Page 5");
			PageManager::set(5);
		}

		// アニメーション更新
		angle += 2.0f;
		if (angle >= 360.0f)
			angle -= 360.0f;

		// 1. 矩形: 回転、脈動（拡大縮小）
		shape1.setRotation(angle);
		float s1 = 1.0f + 0.3f * sin(millis() / 200.0f);
		shape1.setScale(s1, s1);

		// 2. 円: 逆回転（ピボットが端なので偏心回転する）
		shape2.setRotation(-angle * 1.5f);

		// 3. カスタム図形: 平行移動、脈動
		float tx = 10.0f * cos(millis() / 300.0f);
		float ty = 10.0f * sin(millis() / 300.0f);
		shape3.setTranslation(tx, ty);
		float s3 = 1.0f + 0.4f * sin(millis() / 400.0f);
		shape3.setScale(s3, s3);
	}

	void init() override
	{
		uilib::Page::init({M5.Lcd.width(), M5.Lcd.height()}, BLACK, &M5.Lcd, 16);

		// タイトルラベル
		uilib::Color title_fg(255, 255, 255);
		uilib::Color title_bg = uilib::Color::fromHSV(200.0f, 1.0f, 0.4f);
		label1 = uilib::Label("Page4 Shapes", {160, 15}, title_fg, title_bg, 1.0f, this, &fonts::lgfxJapanGothic_20, 4, 4);
		label1.setAlign(uilib::Align::CENTER);

		// 遷移ボタン（右下配置）
		button3 = uilib::DButton("Change", {220, 195}, BLUE, WHITE, 1.0f, this, &fonts::lgfxJapanGothic_20, 10, 4);

		// 図形は絶対座標配置
		// 1. 矩形 (垂直グラデーション、ピボットは中心)
		shape1 = uilib::Shape(uilib::ShapeType::Rectangle, {60, 110}, 40, 40, this);
		shape1.setGradient(uilib::GradientType::LinearVertical, uilib::Color(255, 0, 0), uilib::Color(0, 0, 255));
		shape1.setOrigin(20.0f, 20.0f);

		// 2. 円 (水平グラデーション、ピボットは端にずらす)
		shape2 = uilib::Shape(uilib::ShapeType::Circle, {160, 110}, 40, 40, this);
		shape2.setGradient(uilib::GradientType::LinearHorizontal, uilib::Color(255, 255, 0), uilib::Color(0, 255, 255));
		shape2.setOrigin(0.0f, 20.0f);

		// 3. カスタム図形 (星形、ピボットは中心)
		shape3 = uilib::Shape(uilib::ShapeType::Custom, {260, 110}, 40, 40, this);
		shape3.setOrigin(20.0f, 20.0f);
		shape3.setCustomDrawCallback([](M5Canvas *sprite, int32_t w, int32_t h)
									 {
			float cx = w / 2.0f;
			float cy = h / 2.0f;
			float r_out = w / 2.0f;
			float r_in = r_out * 0.4f;
			
			uilib::point points[10];
			for (int i = 0; i < 10; ++i) {
				float a = -M_PI_2 + i * M_PI / 5.0f;
				float r = (i % 2 == 0) ? r_out : r_in;
				points[i] = { static_cast<int32_t>(cx + r * cos(a)), static_cast<int32_t>(cy + r * sin(a)) };
			}
			
			for (int i = 0; i < 10; ++i) {
				sprite->fillTriangle(
					cx, cy,
					points[i].x, points[i].y,
					points[(i + 1) % 10].x, points[(i + 1) % 10].y,
					YELLOW
				);
			}
			for (int i = 0; i < 10; ++i) {
				sprite->drawLine(
					points[i].x, points[i].y,
					points[(i + 1) % 10].x, points[(i + 1) % 10].y,
					RED
				);
			} });

		// 描画順序を考慮してPageに追加
		addElement(&label1);
		addElement(&button3);
		addElement(&shape1);
		addElement(&shape2);
		addElement(&shape3);

		PageManager::add(this, 4);
	}
};

class Page5 : public uilib::Page
{
private:
	uilib::Label label1;
	uilib::Label label2;
	uilib::DButton button3;
	uilib::Shape shape1;
	uilib::Shape shape2;
	uilib::Shape shape3;
	uilib::VBox vbox;
	uilib::HBox hbox;
	float angle = 0.0f;

public:
	void loop() override
	{
		if (button3.wasReleased())
		{
			Serial.println("Next button released! Moving to Page 6");
			PageManager::set(6);
		}

		// アニメーション更新（Page5でも少し動かしてアフィン変換がレイアウト上で正しく動くか検証）
		angle += 1.5f;
		if (angle >= 360.0f)
			angle -= 360.0f;

		shape1.setRotation(angle);
		shape2.setRotation(-angle);
		float s3 = 1.0f + 0.3f * sin(millis() / 300.0f);
		shape3.setScale(s3, s3);
	}

	void init() override
	{
		uilib::Page::init({M5.Lcd.width(), M5.Lcd.height()}, BLACK, &M5.Lcd, 16);

		// タイトルラベル
		uilib::Color title_fg(255, 255, 255);
		uilib::Color title_bg = uilib::Color::fromHSV(120.0f, 1.0f, 0.4f);
		label1 = uilib::Label("Page5 Layout", {160, 15}, title_fg, title_bg, 1.0f, this, &fonts::lgfxJapanGothic_20, 4, 4);
		label1.setAlign(uilib::Align::CENTER);

		// 左側 VBox (Control Panel 用)
		vbox = uilib::VBox({10, 50}, 130, 180, 0x2104, this);
		vbox.setStrokeWidth(2);
		vbox.setBorderColor(uilib::Color(100, 100, 255));
		vbox.setPadding(8, 8, 8, 8);
		vbox.setSpacing(12);

		// VBox 内の要素
		label2 = uilib::Label("Control Panel", {0, 0}, WHITE, 0x2104, 1.0f, this, &fonts::efontJA_14);
		label2.setAlign(uilib::Align::CENTER);
		button3 = uilib::DButton("Change", {220, 195}, BLUE, WHITE, 1.0f, this, &fonts::lgfxJapanGothic_20, 10, 4);

		vbox.addChild(&label2);

		// 右側 HBox (Shape Container 用)
		hbox = uilib::HBox({150, 50}, 160, 180, 0x0841, this);
		hbox.setStrokeWidth(1);
		hbox.setBorderColor(uilib::Color(255, 100, 100));
		hbox.setPadding(6, 6, 6, 6);
		hbox.setSpacing(10);

		// HBox 内の要素 (図形)
		// 1. 矩形 (垂直グラデーション、ピボットは中心)
		shape1 = uilib::Shape(uilib::ShapeType::Rectangle, {0, 0}, 40, 40, this);
		shape1.setGradient(uilib::GradientType::LinearVertical, uilib::Color(255, 0, 0), uilib::Color(0, 0, 255));
		shape1.setOrigin(20.0f, 20.0f);

		// 2. 円 (水平グラデーション、ピボットは端にずらす)
		shape2 = uilib::Shape(uilib::ShapeType::Circle, {0, 0}, 40, 40, this);
		shape2.setGradient(uilib::GradientType::LinearHorizontal, uilib::Color(255, 255, 0), uilib::Color(0, 255, 255));
		shape2.setOrigin(0.0f, 20.0f);

		// 3. カスタム図形 (星形、ピボットは中心)
		shape3 = uilib::Shape(uilib::ShapeType::Custom, {0, 0}, 40, 40, this);
		shape3.setOrigin(20.0f, 20.0f);
		shape3.setCustomDrawCallback([](M5Canvas *sprite, int32_t w, int32_t h)
									 {
			float cx = w / 2.0f;
			float cy = h / 2.0f;
			float r_out = w / 2.0f;
			float r_in = r_out * 0.4f;
			
			uilib::point points[10];
			for (int i = 0; i < 10; ++i) {
				float a = -M_PI_2 + i * M_PI / 5.0f;
				float r = (i % 2 == 0) ? r_out : r_in;
				points[i] = { static_cast<int32_t>(cx + r * cos(a)), static_cast<int32_t>(cy + r * sin(a)) };
			}
			
			for (int i = 0; i < 10; ++i) {
				sprite->fillTriangle(
					cx, cy,
					points[i].x, points[i].y,
					points[(i + 1) % 10].x, points[(i + 1) % 10].y,
					YELLOW
				);
			}
			for (int i = 0; i < 10; ++i) {
				sprite->drawLine(
					points[i].x, points[i].y,
					points[(i + 1) % 10].x, points[(i + 1) % 10].y,
					RED
				);
			} });

		hbox.addChild(&shape1);
		hbox.addChild(&shape2);
		hbox.addChild(&shape3);

		// 描画順序を考慮してPageに追加
		addElement(&label1);
		addElement(&vbox);
		addElement(&hbox);
		addElement(&label2);
		addElement(&button3);
		addElement(&shape1);
		addElement(&shape2);
		addElement(&shape3);

		PageManager::add(this, 5);
	}
};

class Page6 : public uilib::Page
{
private:
	uilib::Label label_title;
	uilib::Label label_input;
	uilib::Grid grid;

	uilib::DButton btn1, btn2, btn3;
	uilib::DButton btn4, btn5, btn6;
	uilib::DButton btn7, btn8, btn9;
	uilib::DButton btn_clear, btn0, btn_change;

	String input_val = "0";

public:
	void loop() override
	{
		if (btn1.wasReleased())
		{
			input_val = (input_val == "0") ? "1" : input_val + "1";
			label_input.set(input_val);
		}
		if (btn2.wasReleased())
		{
			input_val = (input_val == "0") ? "2" : input_val + "2";
			label_input.set(input_val);
		}
		if (btn3.wasReleased())
		{
			input_val = (input_val == "0") ? "3" : input_val + "3";
			label_input.set(input_val);
		}
		if (btn4.wasReleased())
		{
			input_val = (input_val == "0") ? "4" : input_val + "4";
			label_input.set(input_val);
		}
		if (btn5.wasReleased())
		{
			input_val = (input_val == "0") ? "5" : input_val + "5";
			label_input.set(input_val);
		}
		if (btn6.wasReleased())
		{
			input_val = (input_val == "0") ? "6" : input_val + "6";
			label_input.set(input_val);
		}
		if (btn7.wasReleased())
		{
			input_val = (input_val == "0") ? "7" : input_val + "7";
			label_input.set(input_val);
		}
		if (btn8.wasReleased())
		{
			input_val = (input_val == "0") ? "8" : input_val + "8";
			label_input.set(input_val);
		}
		if (btn9.wasReleased())
		{
			input_val = (input_val == "0") ? "9" : input_val + "9";
			label_input.set(input_val);
		}
		if (btn0.wasReleased())
		{
			input_val = (input_val == "0") ? "0" : input_val + "0";
			label_input.set(input_val);
		}

		if (btn_clear.wasReleased())
		{
			input_val = "0";
			label_input.set(input_val);
		}

		if (btn_change.wasReleased())
		{
			Serial.println("Next button released! Moving to Page 7");
			PageManager::set(7);
		}
	}

	void init() override
	{
		uilib::Page::init({M5.Lcd.width(), M5.Lcd.height()}, BLACK, &M5.Lcd, 16);

		// タイトルラベル
		uilib::Color title_fg(255, 255, 255);
		uilib::Color title_bg = uilib::Color::fromHSV(280.0f, 1.0f, 0.4f);
		label_title = uilib::Label("Page6 Grid Test", {160, 15}, title_fg, title_bg, 1.0f, this, &fonts::lgfxJapanGothic_20, 4, 4);
		label_title.setAlign(uilib::Align::CENTER);

		// 入力結果表示ラベル (右側)
		label_input = uilib::Label("0", {250, 120}, WHITE, 0x18C3, 1.0f, this, &fonts::efontJA_16, 8, 4);
		label_input.setAlign(uilib::Align::CENTER);

		// 4行3列のグリッド (左側)
		grid = uilib::Grid({10, 50}, 170, 180, 4, 3, 0x1082, this);
		grid.setStrokeWidth(2);
		grid.setBorderColor(uilib::Color(150, 150, 255));
		grid.setPadding(6, 6, 6, 6);
		grid.setSpacing(6, 6);

		// ボタン初期化 (横マージン=12, 縦マージン=4, 角丸=4 とし、セルいっぱいに広げて押しやすくする)
		uilib::Color btn_bg(100, 100, 100);
		btn1 = uilib::DButton("1", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 12, 4, -1, -1, 4);
		btn2 = uilib::DButton("2", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 12, 4, -1, -1, 4);
		btn3 = uilib::DButton("3", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 12, 4, -1, -1, 4);
		btn4 = uilib::DButton("4", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 12, 4, -1, -1, 4);
		btn5 = uilib::DButton("5", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 12, 4, -1, -1, 4);
		btn6 = uilib::DButton("6", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 12, 4, -1, -1, 4);
		btn7 = uilib::DButton("7", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 12, 4, -1, -1, 4);
		btn8 = uilib::DButton("8", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 12, 4, -1, -1, 4);
		btn9 = uilib::DButton("9", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 12, 4, -1, -1, 4);
		btn_clear = uilib::DButton("Clr", {0, 0}, WHITE, RED, 1.0f, this, &fonts::efontJA_14, 12, 4, -1, -1, 4);
		btn0 = uilib::DButton("0", {0, 0}, WHITE, btn_bg, 1.0f, this, &fonts::efontJA_14, 12, 4, -1, -1, 4);
		btn_change = uilib::DButton("Chg", {0, 0}, WHITE, BLUE, 1.0f, this, &fonts::efontJA_14, 12, 4, -1, -1, 4);

		grid.addChild(&btn1);
		grid.addChild(&btn2);
		grid.addChild(&btn3);
		grid.addChild(&btn4);
		grid.addChild(&btn5);
		grid.addChild(&btn6);
		grid.addChild(&btn7);
		grid.addChild(&btn8);
		grid.addChild(&btn9);
		grid.addChild(&btn_clear);
		grid.addChild(&btn0);
		grid.addChild(&btn_change);

		// Pageへ各要素を登録 (グリッドとラベルは個別登録、子要素ボタンも登録が必要)
		addElement(&label_title);
		addElement(&label_input);
		addElement(&grid);

		// ボタンもページに登録
		addElement(&btn1);
		addElement(&btn2);
		addElement(&btn3);
		addElement(&btn4);
		addElement(&btn5);
		addElement(&btn6);
		addElement(&btn7);
		addElement(&btn8);
		addElement(&btn9);
		addElement(&btn_clear);
		addElement(&btn0);
		addElement(&btn_change);

		PageManager::add(this, 6);
	}
};

class Page7 : public uilib::Page
{
private:
	uilib::Label label_title;
	uilib::ScrollPanel scroll_panel_v;
	uilib::ScrollPanel scroll_panel_h;
	uilib::ScrollPanel scroll_panel_both;
	uilib::VBox vbox;
	uilib::HBox hbox;
	uilib::Grid grid;

	uilib::DButton btn_v[5];
	uilib::DButton btn_h[5];
	uilib::DButton btn_both[6];
	uilib::DButton btn_change;

public:
	void loop() override
	{
		if (btn_change.wasReleased())
		{
			Serial.println("Change button released! Moving to Page 8");
			PageManager::set(8);
		}

		for (int i = 0; i < 5; ++i)
		{
			if (btn_v[i].wasReleased())
			{
				Serial.printf("V Item %d selected\n", i + 1);
			}
			if (btn_h[i].wasReleased())
			{
				Serial.printf("H Item %d selected\n", i + 1);
			}
		}
		for (int i = 0; i < 6; ++i)
		{
			if (btn_both[i].wasReleased())
			{
				Serial.printf("Both Item %d selected\n", i + 1);
			}
		}
	}

	void init() override
	{
		uilib::Page::init({M5.Lcd.width(), M5.Lcd.height()}, BLACK, &M5.Lcd, 16);

		// タイトルラベル
		uilib::Color title_fg(255, 255, 255);
		uilib::Color title_bg = uilib::Color::fromHSV(180.0f, 1.0f, 0.4f);
		label_title = uilib::Label("Page7 Scroll Test", {160, 15}, title_fg, title_bg, 1.0f, this, &fonts::lgfxJapanGothic_20, 4, 4);
		label_title.setAlign(uilib::Align::CENTER);

		// 1. 縦スクロールパネル (X:15, Y:50, W:80, H:130)
		scroll_panel_v = uilib::ScrollPanel({15, 50}, 80, 130, 0x1082, this);
		scroll_panel_v.setStrokeWidth(2);
		scroll_panel_v.setBorderColor(uilib::Color(100, 100, 255));
		scroll_panel_v.setScrollDirection(uilib::ScrollDirection::VERTICAL);

		// VBox (ScrollPanelの内側に合わせる X:17, Y:52, W:76, H:220)
		vbox = uilib::VBox({17, 52}, 76, 220, 0x1082, this);
		vbox.setPadding(4, 4, 4, 4);
		vbox.setSpacing(8);
		vbox.setStrokeWidth(0);

		// 縦スクロールボタンの初期化
		uilib::Color btn_v_bg(50, 150, 50);
		for (int i = 0; i < 5; ++i)
		{
			btn_v[i] = uilib::DButton("V " + String(i + 1), {0, 0}, WHITE, btn_v_bg, 1.0f, this, &fonts::efontJA_14, 6, 2);
			vbox.addChild(&btn_v[i]);
		}
		scroll_panel_v.addChild(&vbox);
		for (int i = 0; i < 5; ++i)
		{
			scroll_panel_v.addChild(&btn_v[i]);
		}

		// 2. 横スクロールパネル (X:110, Y:50, W:195, H:60)
		scroll_panel_h = uilib::ScrollPanel({110, 50}, 195, 60, 0x1082, this);
		scroll_panel_h.setStrokeWidth(2);
		scroll_panel_h.setBorderColor(uilib::Color(255, 100, 100));
		scroll_panel_h.setScrollDirection(uilib::ScrollDirection::HORIZONTAL);

		// HBox (ScrollPanelの内側に合わせる X:112, Y:52, W:350, H:56)
		hbox = uilib::HBox({112, 52}, 350, 56, 0x1082, this);
		hbox.setPadding(4, 4, 4, 4);
		hbox.setSpacing(8);
		hbox.setStrokeWidth(0);

		// 横スクロールボタンの初期化
		uilib::Color btn_h_bg(150, 50, 150);
		for (int i = 0; i < 5; ++i)
		{
			btn_h[i] = uilib::DButton("H " + String(i + 1), {0, 0}, WHITE, btn_h_bg, 1.0f, this, &fonts::efontJA_14, 6, 2);
			hbox.addChild(&btn_h[i]);
		}
		scroll_panel_h.addChild(&hbox);
		for (int i = 0; i < 5; ++i)
		{
			scroll_panel_h.addChild(&btn_h[i]);
		}

		// 3. 縦横スクロールパネル (X:110, Y:120, W:195, H:60)
		scroll_panel_both = uilib::ScrollPanel({110, 120}, 195, 60, 0x1082, this);
		scroll_panel_both.setStrokeWidth(2);
		scroll_panel_both.setBorderColor(uilib::Color(100, 255, 255));
		scroll_panel_both.setScrollDirection(uilib::ScrollDirection::BOTH);

		// Grid (ScrollPanelの内側に合わせる X:112, Y:122, W:300, H:100, 2行3列)
		grid = uilib::Grid({112, 122}, 300, 100, 2, 3, 0x1082, this);
		grid.setPadding(4, 4, 4, 4);
		grid.setSpacing(10, 10);
		grid.setStrokeWidth(0);

		// 縦横スクロールボタンの初期化
		uilib::Color btn_both_bg(50, 100, 150);
		for (int i = 0; i < 6; ++i)
		{
			btn_both[i] = uilib::DButton("B " + String(i + 1), {0, 0}, WHITE, btn_both_bg, 1.0f, this, &fonts::efontJA_14, 6, 2);
			grid.addChild(&btn_both[i]);
		}
		scroll_panel_both.addChild(&grid);
		for (int i = 0; i < 6; ++i)
		{
			scroll_panel_both.addChild(&btn_both[i]);
		}

		// Changeボタン (X:220, Y:195)
		btn_change = uilib::DButton("Change", {220, 195}, BLUE, WHITE, 1.0f, this, &fonts::lgfxJapanGothic_20, 10, 4);

		// ページに登録
		addElement(&label_title);
		addElement(&scroll_panel_v);
		addElement(&scroll_panel_h);
		addElement(&scroll_panel_both);
		addElement(&btn_change);

		PageManager::add(this, 7);
	}
};

class Page8 : public uilib::Page
{
private:
	uilib::Label label_title;
	uilib::TabPanel tab_panel;

	// タブ1用
	uilib::VBox vbox_tab1;
	uilib::DButton btn_control1;
	uilib::ToggleSwitch toggle_control;

	// タブ2用
	uilib::HBox hbox_tab2;
	uilib::DButton btn_layout1;
	uilib::Shape shape_layout;

	// タブ3用 (スクロールデモ)
	uilib::ScrollPanel scroll_tab3;
	uilib::VBox vbox_scroll_items;
	uilib::DButton btn_scroll_items[6];

	// メッセージボックス
	uilib::MessageBox *message_box = nullptr;

	uilib::DButton btn_change;

public:
	~Page8()
	{
		delete message_box;
	}

	void loop() override
	{
		if (btn_change.wasReleased())
		{
			Serial.println("Change button released! Returning to Page 1");
			PageManager::set(1);
		}

		if (btn_control1.wasReleased())
		{
			Serial.println("Control tab: Button A released! Showing MessageBox");
			message_box->show("Confirm Action", "Do you want to proceed\nwith this task?", uilib::MessageBoxType::OK_CANCEL, [](uilib::MessageBoxResult result)
							  {
				if (result == uilib::MessageBoxResult::OK) {
					Serial.println("MessageBox Result: OK selected");
				} else {
					Serial.println("MessageBox Result: CANCEL selected");
				} });
		}

		if (btn_layout1.wasReleased())
		{
			Serial.println("Layout tab: button 1 selected");
		}

		for (int i = 0; i < 6; ++i)
		{
			if (btn_scroll_items[i].wasReleased())
			{
				Serial.printf("Scroll tab: item %d selected\n", i + 1);
			}
		}

		// アニメーション更新（タブ2の図形を回転）
		static float angle = 0.0f;
		angle += 2.0f;
		if (angle >= 360.0f)
			angle -= 360.0f;
		shape_layout.setRotation(angle);
	}

	void init() override
	{
		uilib::Page::init({M5.Lcd.width(), M5.Lcd.height()}, BLACK, &M5.Lcd, 16);

		// タイトルラベル
		uilib::Color title_fg(255, 255, 255);
		uilib::Color title_bg = uilib::Color::fromHSV(240.0f, 1.0f, 0.4f);
		label_title = uilib::Label("Page8 Tab Test", {160, 15}, title_fg, title_bg, 1.0f, this, &fonts::lgfxJapanGothic_20, 4, 4);
		label_title.setAlign(uilib::Align::CENTER);

		// タブパネルの配置 (X:15, Y:50, W:290, H:135)
		tab_panel = uilib::TabPanel({15, 50}, 290, 135, 0x1082, this);
		tab_panel.setStrokeWidth(2);
		tab_panel.setBorderColor(uilib::Color(100, 255, 100));

		// --- タブ1: Control Panel (VBox) ---
		// コンテンツ幅286に対し, ボタン幅が約120pxになるため, 左パディングを83pxにして中央寄せに配置
		// 縦方向ははみ出しを防ぐため上余白8px、下余白8px、間隔8pxに調整
		vbox_tab1 = uilib::VBox({0, 0}, 100, 100, 0x1082, this);
		vbox_tab1.setPadding(83, 10, 8, 8);
		vbox_tab1.setSpacing(8);
		vbox_tab1.setStrokeWidth(0);

		// 第12引数(margin_y)を明示的に6に指定して、ボタンの縦幅を適正(26px)にし、はみ出しを修正
		btn_control1 = uilib::DButton("Button A", {0, 0}, WHITE, uilib::Color(50, 120, 200), 1.0f, this, &fonts::efontJA_14, 24, 6, -1, -1, 6);
		toggle_control = uilib::ToggleSwitch({0, 0}, {60, 28}, false, GREEN, 0x7BEF, WHITE, this);

		vbox_tab1.addChild(&btn_control1);
		vbox_tab1.addChild(&toggle_control);

		// --- タブ2: Layout Panel (HBox) ---
		// コンテンツ幅286に対し、ボタン幅約80px、形状36px、間隔45pxとし、左パディングを55pxにして左右のバランスを調整
		// 縦方向は中央に揃えるため上余白を32pxに設定
		hbox_tab2 = uilib::HBox({0, 0}, 100, 100, 0x1082, this);
		hbox_tab2.setPadding(55, 10, 32, 10);
		hbox_tab2.setSpacing(45);
		hbox_tab2.setStrokeWidth(0);

		btn_layout1 = uilib::DButton("Btn B", {0, 0}, WHITE, uilib::Color(180, 60, 60), 1.0f, this, &fonts::efontJA_14, 20, 6, -1, -1, 6);
		shape_layout = uilib::Shape(uilib::ShapeType::Rectangle, {0, 0}, 36, 36, this);
		shape_layout.setGradient(uilib::GradientType::LinearVertical, uilib::Color(RED), uilib::Color(BLUE));
		shape_layout.setOrigin(18.0f, 18.0f);

		hbox_tab2.addChild(&btn_layout1);
		hbox_tab2.addChild(&shape_layout);

		// --- タブ3: Scroll Panel (VBox in ScrollPanel) ---
		scroll_tab3 = uilib::ScrollPanel({0, 0}, 100, 100, 0x0841, this);
		scroll_tab3.setStrokeWidth(0);
		scroll_tab3.setScrollDirection(uilib::ScrollDirection::VERTICAL);

		// スクロール内に配置する縦並びコンテナ (幅286px、高さ220pxに設定してスクロールを有効化)
		vbox_scroll_items = uilib::VBox({0, 0}, 286, 220, 0x0841, this);
		vbox_scroll_items.setPadding(83, 10, 6, 6);
		vbox_scroll_items.setSpacing(8);
		vbox_scroll_items.setStrokeWidth(0);

		for (int i = 0; i < 6; ++i)
		{
			btn_scroll_items[i] = uilib::DButton("Item " + String(i + 1), {0, 0}, WHITE, uilib::Color(70, 70, 70), 1.0f, this, &fonts::efontJA_14, 24, 6, -1, -1, 6);
			vbox_scroll_items.addChild(&btn_scroll_items[i]);
		}
		scroll_tab3.addChild(&vbox_scroll_items);

		// メッセージボックスの初期化 (X:40, Y:55, W:240, H:130 - 動的確保でアドレスを固定)
		message_box = new uilib::MessageBox({40, 55}, 240, 130, 0x2104, this);

		// タブを追加 (VBox / HBox / ScrollPanel を直接登録)
		tab_panel.addTab("Control", &vbox_tab1);
		tab_panel.addTab("Layout", &hbox_tab2);
		tab_panel.addTab("Scroll", &scroll_tab3);

		// Changeボタン
		btn_change = uilib::DButton("Change", {220, 195}, BLUE, WHITE, 1.0f, this, &fonts::lgfxJapanGothic_20, 10, 4);

		// Pageへ登録
		addElement(&label_title);
		addElement(&tab_panel);
		addElement(&btn_change);
		addElement(message_box); // 最前面に重ねて描画させるために最後に追加

		PageManager::add(this, 8);
	}
};

class Page9 : public uilib::Page
{
private:
	uilib::Label label_title;
	uilib::Label label_input_box;
	uilib::DButton btn_clear;
	uilib::DButton btn_back;
	uilib::Keyboard *keyboard = nullptr;
	uilib::MessageBox *message_box = nullptr;

public:
	~Page9()
	{
		delete keyboard;
		delete message_box;
	}

	void loop() override
	{
		if (btn_back.wasReleased())
		{
			Serial.println("Back button released! Returning to Home Page (Page 0)");
			PageManager::set(0);
		}
		if (btn_clear.wasReleased())
		{
			Serial.println("Clear button released! Clearing keyboard text");
			if (keyboard)
			{
				keyboard->clear();
			}
		}
	}

	void init() override
	{
		uilib::Page::init({M5.Lcd.width(), M5.Lcd.height()}, BLACK, &M5.Lcd, 16);

		// タイトルラベル (Green background)
		uilib::Color title_fg(255, 255, 255);
		uilib::Color title_bg = uilib::Color::fromHSV(120.0f, 1.0f, 0.4f);
		label_title = uilib::Label("Keyboard Demo", {160, 15}, title_fg, title_bg, 1.0f, this, &fonts::lgfxJapanGothic_20, 4, 4);
		label_title.setAlign(uilib::Align::CENTER);

		// 入力テキスト表示ボックス (Placeholder text)
		label_input_box = uilib::Label("Tap keys to type...", {15, 55}, WHITE, 0x18C3, 1.0f, this, &fonts::efontJA_14, 6, 4);
		label_input_box.setRadius(2);

		// クリアボタン
		btn_clear = uilib::DButton("Clear", {215, 55}, WHITE, 0xA000, 1.0f, this, &fonts::efontJA_14, 8, 4);

		// 戻るボタン
		btn_back = uilib::DButton("Back", {275, 55}, WHITE, 0x4A69, 1.0f, this, &fonts::efontJA_14, 8, 4);

		// キーボードの配置
		keyboard = new uilib::Keyboard({0, 120}, this);
		keyboard->setTargetLabel(&label_input_box);

		// メッセージボックス
		message_box = new uilib::MessageBox({40, 55}, 240, 130, 0x2104, this);

		// 決定(Enter)時のコールバック
		keyboard->setOnSubmitCallback([this](const String& text) {
			Serial.printf("Keyboard submitted: %s\n", text.c_str());
			message_box->show("Submitted Text", "You entered:\n" + (text.isEmpty() ? String("(empty)") : text), uilib::MessageBoxType::OK, [this](uilib::MessageBoxResult result) {
				message_box->hide();
			});
		});

		// ページへ登録
		addElement(&label_title);
		addElement(&label_input_box);
		addElement(&btn_clear);
		addElement(&btn_back);
		addElement(keyboard);
		addElement(message_box); // 最前面に重ねるために最後に追加

		PageManager::add(this, 9);
	}
};

PageHome page_home;
Page1 page_1;
Page2 page_2;
Page3 page_3;
Page4 page_4;
Page5 page_5;
Page6 page_6;
Page7 page_7;
Page8 page_8;
Page9 page_9;

void setup()
{
	auto cfg = M5.config();
	M5.begin(cfg);
	M5.Lcd.fillScreen(BLACK);
	Serial.begin(115200);
	delay(1000); // Wait for serial to initialize
	Serial.println("Serial initialized");
	page_home.init();
	page_1.init();
	page_2.init();
	page_3.init();
	page_4.init();
	page_5.init();
	page_6.init();
	page_7.init();
	page_8.init();
	page_9.init();
	PageManager::set(0);
	Serial.println("Initialized");
}

void loop()
{
	M5.update();

	if (M5.BtnB.wasReleased())
	{
		Serial.println("Home button (BtnB) released! Moving to Home Page (Page 0)");
		PageManager::set(0);
	}

	PageManager::update();
}