#include "Game.h"

#include <cmath>

#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_gfxPrimitives_font.h>

#include <noise/noise.h>

#include "DebugTool_Camera.h"
#include "DebugTool_Lighting.h"
#include "DebugTool_Placeholder.h"
#include "DebugTool_RayCast.h"

#include "FontAtlas.h"
#include "RenderTarget.h"
#include "Input.h"
#include "SurfaceDrawing.h"
#include "TextureAtlas.h"

#include "Util.h"

#include "UIWidget_Button.h"
#include "UIWidget_Checkbox.h"
#include "UIWidget_Cycle.h"
#include "UIWidget_DebugToolView.h"
#include "UIWidget_Pane.h"
#include "UIWidget_Line.h"
#include "UIWidget_Label.h"

//#include "UIButton.h"
//#include "UIFrame.h"
//#include "UILabel.h"
//#include "UITextbox.h"
//#include "UIWidget_Test.h"

template<class T>
void Game::_RegDbgTool()
{
	std::type_index TypeIndex = typeid(T);
	if (_dbgToolRegistry.count(TypeIndex) == 0)
	{
		ADebugTool* NewTool = new T(*this);
		_dbgToolRegistry.insert({ TypeIndex, NewTool });
		_dbgTools.push_back(NewTool);
	}
}

template<class T>
ADebugTool* Game::_DbgTool()
{
	static size_t TypeCounter = _dbgTools.size();

	ADebugTool* NewTool = new T();
}

SDL_Texture* GenNoise(SDL_Renderer* Ren)
{
	SDL_Surface* Surf = SDL_CreateRGBSurface(0, 1600, 120, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000);
	SDL_FillRect(Surf, nullptr, SDL_MapRGB(Surf->format, 0, 0, 0));

	noise::module::Perlin perlin;
	
	perlin.SetSeed(8008);
	for (int x = 0; x < 1600; x++)
	{
		
		// Big curves
		// [-1, 1]
		double Val1 = perlin.GetValue(2.9 + static_cast<double>(x) * 0.002, 10.1, 1.0);
		// [0, 2]
		Val1 += 1.0;
		// [0, 1]
		Val1 /= 2.0;

		// Small curves
		// [-1, 1]
		double Val2 = perlin.GetValue(0.02 + static_cast<double>(x) * 0.04, 6.4, 1.0);
		// [0, 2]
		//Val2 += 1.0;
		// [0, 1]
		//Val2 /= 2.0;
		

		int LineHeight = (90 * Val1) + (7 * Val2);

		int BottomY = 119;
		int BufferSpace = 2;
		int TopY = BottomY - BufferSpace - LineHeight;

		Draw::VertLine(Surf, x, TopY, BottomY, { 255,127,0,0 });
	}

	Draw::SetPixel(Surf, 0, 0, { 255, 0, 0, 255 });
	Draw::SetPixel(Surf, 0, 119, { 255, 0, 0, 255 });

	SDL_Texture* ToRet = SDL_CreateTextureFromSurface(Ren, Surf);
	SDL_FreeSurface(Surf);
	return ToRet;
}

Game::Game(Window* Win) :
	_Window(Win)
	//_sceneDebug(320, 240)
	//_Window("Hadal")
{
	//_Window(WindowWidth, WindowHeight, "Hadal");
	_Window->SetSize(WindowWidth, WindowHeight);
	_scene = new UIScene({ 0, 0, WindowWidth, WindowHeight / 4 });
	_scene->SetVisibility(false);
	//_scene->AddWidget(new AUIWidget());
	//_scene->AddWidget(new AUIContainerWidget());

	UIWidget_Pane* TestPane = new UIWidget_Pane("Pane", true);
	//UIWidgetHandle TestPane = std::make_unique<UIWidget_Pane>("Pane", true);
	//std::unique_ptr<void*>::un
	//UIWidget_Pane* TestPane = static_cast<UIWidget_Pane*>(TestPaneHandle.get());
	TestPane->SetHeight(300);

	AUIWidget* Widget1 = new AUIWidget();
	TestPane->AddWidget(Widget1);

	TestPane->AddWidget(new UIWidget_Line({ 155, 155, 155, 255 }));

	UIWidget_Label* LabelLeft = new UIWidget_Label("Left", { 255, 255, 255, 255 });
	TestPane->AddWidget(LabelLeft);

	UIWidget_Label* LabelCenter = new UIWidget_Label("Center", { 255, 255, 255, 255 }, FC_ALIGN_CENTER);
	TestPane->AddWidget(LabelCenter);

	UIWidget_Label* LabelRight = new UIWidget_Label("Right", { 255, 255, 255, 255 }, FC_ALIGN_RIGHT);
	TestPane->AddWidget(LabelRight);

	UIWidget_Label* LabelWrap = new UIWidget_Label("This is going to be some very long text that wraps around.", { 255, 255, 255, 255 }, FC_ALIGN_LEFT);
	TestPane->AddWidget(LabelWrap);

	TestPane->AddWidget(new UIWidget_Line({ 155, 155, 155, 255 }));

	UIWidget_Button* Widget2 = new UIWidget_Button(FontAtlas::FindFont("default_bold"), "Test Button");
	TestPane->AddWidget(Widget2);

	auto Widget3CB = [](UIWidget_Button* Caller) {
		int i = 3;
	};
	UIWidget_Button* Widget3 = new UIWidget_Button(FontAtlas::FindFont("default_bold"), "Test Button 2", Widget3CB);
	TestPane->AddWidget(Widget3);
	//delete Widget3;

	TestPane->AddWidget(new UIWidget_Line({ 155, 155, 155, 255 }));

	UIWidget_Cycle* Widget4 = new UIWidget_Cycle([](UIWidget_Cycle* Caller, void* Userdata) {
		auto SelItem = Caller->SelectedItem();
	});
	Widget4->PushItem("Label 1");
	Widget4->PushItem("Label 2");
	Widget4->PushItem("Label 3");
	Widget4->PushItem("Label 4");
	TestPane->AddWidget(Widget4);

	UIWidget_Checkbox* ChkBox = new UIWidget_Checkbox("Sussy", { 255, 255, 255, 255 });
	TestPane->AddWidget(ChkBox);

	_scene->AddWidget(TestPane);

	//_sceneDebug.AddRootWidget(new UIWidget_Test());
	/*UIFrame* Frame = new UIFrame({ 10, 170, 100, 50 });
	Frame->AddChild(new UILabel({}, "default_bold", "Test Text"));
	Frame->AddChild(new UILabel({}, "default_bold", "Test Text 2\nNow with line breaks!"));
	Frame->AddChild(new UIButton({ 0, 0, 80, 20 }));
	Frame->AddChild(new UIWidget_Test({ 0, 0, 80, 20 }));
	Frame->AddChild(new UITextbox("default_bold"));
	Frame->RecalculateSize();

	_sceneDebug.AddChild(Frame);*/
	//_sceneDebug.AddRootWidget();
	
	
	//_dbgTool_Lighting = new DebugTool_Lighting();
	//_dbgTools.push_back(_dbgTool_Lighting);

	//_dbgTool_Placeholder = new DebugTool_Placeholder();
	//_dbgTools.push_back(_dbgTool_Placeholder);

	//_RegDbgTool<DebugTool_Camera>();
	_RegDbgTool<DebugTool_Lighting>();
	_RegDbgTool<DebugTool_Placeholder>();
	_RegDbgTool<DebugTool_RayCast>();

	/*UIWidgetHandle DebugToolPaneHandle = std::make_unique<UIWidget_Pane>("Debug Tools", true, false);
	UIWidget_Pane* DebugToolPane = static_cast<UIWidget_Pane*>(DebugToolPaneHandle.get());*/
	UIWidget_Pane* DebugToolPane = new UIWidget_Pane("Debug Tools", true, false);
	DebugToolPane->SetHeight(400);
	DebugToolPane->SetWidth(200);
	DebugToolPane->SetOffset({ 200, 20 });

	UIWidget_DebugToolView* TestPane2 = new UIWidget_DebugToolView();

	UIWidget_Cycle* Cycle_DebugTools = new UIWidget_Cycle([this](UIWidget_Cycle* Caller, void* Userdata) {
		UIWidget_DebugToolView* DTV = static_cast<UIWidget_DebugToolView*>(Userdata);
		auto SelItem = Caller->SelectedItem();
		if (SelItem)
		{
			ADebugTool* CurDbgTool = static_cast<ADebugTool*>(SelItem->Value);
			if (this->_currentDebugTool != CurDbgTool)
			{
				if(this->_currentDebugTool)
					this->_currentDebugTool->OnDeactivate();

				CurDbgTool->OnActivate();
				//DTV->SetDescription(CurDbgTool->GetToolDesc());
				DTV->SetTool(CurDbgTool);
				//DTV->
				this->_currentDebugTool = CurDbgTool;
			}
		}
		else
		{
			DTV->SetDescription("( ' v ')");
		}
	}, TestPane2);
	for (auto CurDbgTool : _dbgTools)
		Cycle_DebugTools->PushItem(CurDbgTool->GetToolName(), CurDbgTool);
	DebugToolPane->AddWidget(Cycle_DebugTools);

	//TestPane2->SetVerticalStretch(true);
	DebugToolPane->AddWidget(TestPane2);

	//UIWidget_Label* Label_DbgToolDesc = new UIWidget_Label

	_scene->AddWidget(DebugToolPane);

	/*UIWidgetHandle ScrollbarTestPaneHandle = std::make_unique<UIWidget_Pane>("Scrollbar Test", true, true);
	UIWidget_Pane* ScrollbarTestPane = static_cast<UIWidget_Pane*>(ScrollbarTestPaneHandle.get());*/
	UIWidget_Pane* ScrollbarTestPane = new UIWidget_Pane("Scrollbar Test", true, true);
	{
		ScrollbarTestPane->SetOffset({ 420, 100 });
		UIWidget_Button* AddLabel = new UIWidget_Button(FontAtlas::DefaultFont(), "Add Lump", [ScrollbarTestPane](UIWidget_Button* Caller) {
			ScrollbarTestPane->AddWidget(new UIWidget_Label("Lump ( ' v ')", { 255, 255, 255, 255 }));
			ScrollbarTestPane->Dirty();
		});
		ScrollbarTestPane->AddWidget(AddLabel);

		// TODO: [BUG] When the widget removes itself, its captures are lost because the lambda is destroyed. Copy the value into a local variable before it's destroyed.
		UIWidget_Button* RemoveLabel = new UIWidget_Button(FontAtlas::DefaultFont(), "Remove Lump", [ScrollbarTestPane](UIWidget_Button* Caller) {
			if (ScrollbarTestPane->Children().size() > 3)	// Scrollbar + 2 buttons
			{
				auto ScrollbarTestPanePtr = ScrollbarTestPane;

				const std::unique_ptr<AUIWidget>& LastChild = ScrollbarTestPanePtr->Children().back();
				ScrollbarTestPanePtr->RemoveWidget(LastChild.get());
				//ScrollbarTestPane->AddWidget(new UIWidget_Label("Lump ( ' v ')", { 255, 255, 255, 255 }));
				ScrollbarTestPanePtr->Dirty();
			}
		});
		ScrollbarTestPane->AddWidget(RemoveLabel);
	}
	_scene->AddWidget(ScrollbarTestPane);

	/*
	* 1. Shadow mask (white -> alpha, black -> opaque)	<- WhiteToAlpha
	* 2. Shadow color (whites on mask will be lit)		<- Blend
	*
	* 3. Fullbright
	*/
	WhiteToAlpha = SDL_ComposeCustomBlendMode(
		SDL_BlendFactor::SDL_BLENDFACTOR_ZERO,		// srcFactor
		SDL_BlendFactor::SDL_BLENDFACTOR_ONE,	// dstFactor
		SDL_BlendOperation::SDL_BLENDOPERATION_ADD,	// opRGB

		// A = 1-srcRGB -- The lighter the color, the more transparent?
		// A = (dstA * dstFactorA) + (srcA * 1-srcRGB)
		// A = (1 * 1-srcRGB) + (srcA * 0)
		SDL_BlendFactor::SDL_BLENDFACTOR_ZERO,		// srcFactorA
		SDL_BlendFactor::SDL_BLENDFACTOR_SRC_ALPHA,	// dstFactorA
		SDL_BlendOperation::SDL_BLENDOPERATION_ADD
	);

	int HalfWindowWidth = WindowWidth / 2;
	int HalfWindowHeight = WindowHeight / 2;

	_shadowTarget = new RenderTarget(*_Window, 320, 240);
	//SDL_SetTextureBlendMode(*_shadowTarget, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(*_shadowTarget, WhiteToAlpha);

	_fullbrightTarget = new RenderTarget(*_Window, 320, 240);
	// Set mode, so it'll blend with shadows
	SDL_SetTextureBlendMode(*_fullbrightTarget, SDL_BLENDMODE_BLEND);

	_uiTarget = new RenderTarget(*_Window, 320, 240);
	SDL_SetTextureBlendMode(*_uiTarget, SDL_BLENDMODE_BLEND);

	_debugUITarget = new RenderTarget(*_Window, 640, 480);
	SDL_SetTextureBlendMode(*_debugUITarget, SDL_BLENDMODE_BLEND);

	_screenTarget = new RenderTarget(*_Window, 640, 480);
	SDL_SetTextureBlendMode(*_screenTarget, SDL_BLENDMODE_BLEND);

	//_map->Generate(1600, 2000);
}

void Game::CreateMap()
{
	_map = new Map(*this, 8008);
	_map->SetViewport(_Window->GetWidth() / 2, _Window->GetHeight() / 2);
	_map->Generate(1600, 2000);
}

void Game::Input()
{
	Input::Update();

	float playerAccelX = 0.f, playerAccelY = 0.f;

	if(!Input::ShouldBlockAlphaKeys())
	{
		if(Input::IsKeyDown(SDL_SCANCODE_W))
			playerAccelY -= 25.f;
		if (Input::IsKeyDown(SDL_SCANCODE_S))
			playerAccelY += 25.f;
		if (Input::IsKeyDown(SDL_SCANCODE_A))
			playerAccelX -= 25.f;
		if (Input::IsKeyDown(SDL_SCANCODE_D))
			playerAccelX += 25.f;
	}

	// TODO: If input is being grabbed by a widget, don't do in-game key-state events
	/*const Uint8* Keys = SDL_GetKeyboardState(nullptr);
	if (Keys[SDL_SCANCODE_W])
	{
		playerAccelY = -25.f;
	}
	if (Keys[SDL_SCANCODE_S])
	{
		playerAccelY = 25.f;
	}
	if (Keys[SDL_SCANCODE_A])
	{
		playerAccelX = -25.f;
	}
	if (Keys[SDL_SCANCODE_D])
	{
		playerAccelX = 25.f;
	}*/

	//_map->GetControlledPlayer()->SetAcceleration(playerAccelX, playerAccelY);
	auto PlayerPhys = _map->GetControlledPlayer().GetComponent<Component_Physics>();
	PlayerPhys->AccX = playerAccelX;
	PlayerPhys->AccY = playerAccelY;

	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		if (Event.type == SDL_QUIT)
			_bExitRequested = true;

		// Give debug tools first-order priority in input events
		if (_bDebugToolMode)
			/*if (_dbgTools[_dbgToolIndex]->ProcessInput(Event))
				continue;*/
			if (_currentDebugTool->ProcessInput(Event))
				continue;

		_scene->Input(Event);

		/*if (_sceneDebug.SceneInput(Event))
			continue;*/

		if (Event.type == SDL_EventType::SDL_KEYDOWN)
		{
			bool bCtrl = Event.key.keysym.mod & SDL_Keymod::KMOD_CTRL;
			bool bShift = Event.key.keysym.mod & SDL_Keymod::KMOD_SHIFT;

			// Debug tools controls
			/*if (_bDebugToolMode && !bShift)
			{
				bool bChangedIndex = false;
				switch (Event.key.keysym.sym)
				{
				case SDL_KeyCode::SDLK_LEFTBRACKET:
					_dbgToolIndex = (_dbgToolIndex == 0 ? _dbgTools.size() - 1 : _dbgToolIndex - 1);
					bChangedIndex = true;
					break;
				case SDL_KeyCode::SDLK_RIGHTBRACKET:
					_dbgToolIndex = (_dbgToolIndex == _dbgTools.size() - 1 ? 0 : _dbgToolIndex + 1);
					bChangedIndex = true;
					break;
				}
			}*/

			switch (Event.key.keysym.sym)
			{
			case SDL_KeyCode::SDLK_m:
				_bShowMap = !_bShowMap;
				break;
			case SDL_KeyCode::SDLK_BACKSLASH:
				_scene->SetVisibility(!_scene->bIsVisible());
				_bDebugToolMode = !_bDebugToolMode;
				if (_bDebugToolMode)
				{
					// Just turned debug mode on.
					//_dbgTools[_dbgToolIndex]->Activate();
				}
				else
				{
					// Just turned it off.
					//_dbgTools[_dbgToolIndex]->Deactivate();
				}
				break;
			}
		}
	}
}

void Game::Logic()
{
	_map->Update(1000 / 60);

	//_sceneDebug.SceneLogic();

	// Update camera position to track player
	//_camera.SetPosition(_map->GetControlledPlayer()->GetPosition());

	if (_bDebugToolMode)
		//_dbgTools[_dbgToolIndex]->Logic();
		_currentDebugTool->Logic();
}

void Game::Draw(SDL_Renderer* Ren/*, const SDL_Point& CameraPosCenter*/)
{
	static FC_Font* Font = FontAtlas::FindFont("default_bold");
	/*if (Font == nullptr)
	{
		Font = FC_CreateFont();
		FC_LoadFont(Font, Ren, "./data/font/SFIntermosaic.ttf", 8, { 255, 255, 255, 255 }, TTF_STYLE_BOLD);
	}*/
	//static SDL_Surface* RockTex = SDL_LoadBMP("./data/tex/rocky.bmp");
	//static Sint16 XCoords[] = { 0, 0, 40, 80, 120, 160, 200, 240, 280, 320, 320, };
	//static Sint16 YCoords[] = { 240, 180, 200, 210, 215, 217, 215, 210, 200, 180, 240 };
	//static SDL_Texture* MapTex = nullptr;
	//if (MapTex == nullptr)
	//{
	//	MapTex = GenNoise(Ren);
	//}

	//SDL_SetRenderDrawColor(Ren, 0, 0, 0, 255);
	//SDL_SetRenderDrawColor(Ren, 255, 255, 255, 255);
	//SDL_RenderClear(Ren);

	int x = -1.f, y = -1.f;
	int state = SDL_GetMouseState(&x, &y);

	x /= 2;
	y /= 2;

	float LineLength = 300.f;
	const float BeamWidthDeg = 70.f;
	const float BeamWidthRad = BeamWidthDeg * (M_PI / 180.f);
	// TODO: Draw from sub origin relative to camera, no just the middle of the screen!
	// TODO: [cool] Maybe calculate origin position before screen shakes are applied?
	// Rotation in radians around origin of (160,120)
	float RadianRotation = atan2f(y - 120.f, x - 160.f);
	SDL_Point LineMiddle = {
		static_cast<int>(LineLength * cosf(RadianRotation)) - (0.f * sinf(RadianRotation)),	// x
		static_cast<int>(0 * cosf(RadianRotation)) + (LineLength * sinf(RadianRotation))	// y
	};

	float TopRot = RadianRotation - (BeamWidthRad / 2);
	SDL_Point LineTop = {
		static_cast<int>(LineLength * cosf(TopRot)) - (0.f * sinf(TopRot)),	// x
		static_cast<int>(0 * cosf(TopRot)) + (LineLength * sinf(TopRot))	// y
	};

	float BotRot = RadianRotation + (BeamWidthRad / 2);
	SDL_Point LineBot = {
		static_cast<int>(LineLength * cosf(BotRot)) - (0.f * sinf(BotRot)),	// x
		static_cast<int>(0 * cosf(BotRot)) + (LineLength * sinf(BotRot))	// y
	};

	/*float mouser = atan2f(y - 120.f , x - 160.f);
	float l_x = 100.f, l_y = 0.f;
	
	float ol_x = (l_x * cosf(mouser)) - (l_y * sinf(mouser));
	float ol_y = (l_y * cosf(mouser)) + (l_x * sinf(mouser));*/

	// White 
	_shadowTarget->Bind();
		/*
			By clearing to white, the intensity of the shadows can be increased by lowering the
			value (255 is no coverage, 0 is full coverage)
		*/
		SDL_SetRenderDrawColor(Ren, 255, 255, 255, 255);
		SDL_RenderClear(Ren);
		//static auto SmolScreen = TextureAtlas::FindTexture("p");
		//SDL_SetTextureBlendMode(SmolScreen->GetTexture(), WhiteToAlpha);
		//SDL_RenderCopy(Ren, SmolScreen->GetTexture(), nullptr, nullptr);

		if (state & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			static auto crc = TextureAtlas::FindTexture("crc");
			SDL_Rect CR = { 160 - 15, 120 - 15, 30, 30 };
			SDL_RenderCopy(Ren, crc->GetTexture(), nullptr, &CR);

			/*Sint16 X[3] = { 160, 80, 240 };
			Sint16 Y[3] = { 120, 240, 240 };
			filledPolygonRGBA(Ren, X, Y, 3, 255, 255, 255, 255);*/

			Sint16 X[3] = { 160, 160 + LineTop.x, 160 + LineBot.x };
			Sint16 Y[3] = { 120, 120 + LineTop.y, 120 + LineBot.y };
			filledPolygonRGBA(Ren, X, Y, 3, 255, 255, 255, 255);

			//aapolygonColor(Ren, X, Y, 3, 0xFFFFFFFF);
		}
		
		//SDL_SetRenderDrawColor(Ren, 255, 255, 255, 255);
		//SDL_RenderClear(Ren);
	_shadowTarget->Unbind();


	_fullbrightTarget->Bind();
		// Draw map
		_map->Draw(Ren /*{ _mapOffset, _Depth, 320, 240 }*/);
	_fullbrightTarget->Unbind();

	_uiTarget->Bind();
		SDL_SetRenderDrawColor(Ren, 0, 0, 0, 0);
		SDL_RenderClear(Ren);

		if (_bDebugToolMode)
			//_dbgTools[_dbgToolIndex]->RenderGame(Ren, { 0, 0, 320, 240 });
			_currentDebugTool->RenderGame(Ren, { 0, 0, 320, 240 });
	_uiTarget->Unbind();

	_debugUITarget->Bind();
		SDL_SetRenderDrawColor(Ren, 0, 0, 0, 0);
		SDL_RenderClear(Ren);

		SDL_BlendMode Mode;
		SDL_GetRenderDrawBlendMode(Ren, &Mode);

		_scene->Draw(Ren);

		//if (_bDebugToolMode)
			

		/*if (_bDebugToolMode)
			_sceneDebug.SceneRender(Ren);*/
	_debugUITarget->Unbind();

	_screenTarget->Bind();
		SDL_SetRenderDrawColor(Ren, 0, 0, 0, 255);
		SDL_RenderClear(Ren);

		SDL_RenderCopy(Ren, *_fullbrightTarget, nullptr, nullptr);
		SDL_RenderCopy(Ren, *_shadowTarget, nullptr, nullptr);
		SDL_RenderCopy(Ren, *_uiTarget, nullptr, nullptr);
		SDL_RenderCopy(Ren, *_debugUITarget, nullptr, nullptr);
		
		if (_bDebugToolMode)
		{
			// TODO: Make a Debug UI render target, or don't expect transparency to work.

			//SDL_FPoint PlayerPos = _map->GetControlledPlayer()->GetPositionF();
			SDL_FPoint PlayerPos = *_map->GetControlledPlayer().GetComponent<Component_Position>();
			auto PosBox = FC_Draw(Font, Ren, 10.f, 5.f, "Position:\n X: %.2f\n Y: %.2f\n Depth: %.0f Ft", PlayerPos.x, PlayerPos.y, fabsf(PlayerPos.y));
			lineRGBA(Ren, PosBox.x - 2.f, PosBox.y, PosBox.x - 2.f, PosBox.y + PosBox.h, 255, 255, 255, 255);

			auto FPSBox = FC_Draw(Font, Ren, 10.f, PosBox.y + PosBox.h + 5.f, "FPS:\n %d\n %.2f ms/f", 60, 1000.f / 60.f);
			//lineRGBA(Ren, FPSBox.x, FPSBox.y - 2.f, FPSBox.x + /*FPSBox.w*/ 76.f, FPSBox.y - 2.f, 255, 255, 255, 255);
			lineRGBA(Ren, FPSBox.x - 2.f, FPSBox.y, FPSBox.x - 2.f, FPSBox.y + FPSBox.h, 255, 255, 255, 255);

			SDL_Point MousePos = { 0, 0 };
			SDL_GetMouseState(&MousePos.x, &MousePos.y);
			SDL_Point MapScreenPos = { MousePos.x / 2, MousePos.y / 2 };
			// 65535.0 on position box makes it 76px wide, give it 5px more
			SDL_Point MouseWorldPos = _map->GetCamera().ScreenToWorld(MapScreenPos);
			auto MousePosBox = FC_Draw(Font, Ren, 10.f + 100.f, 5.f, "Mouse Pos:\n X: %d\n Y: %d\nScreen Pos:\n X: %d\n Y: %d\nWorld Pos:\n X: %d\n Y: %d", MapScreenPos.x, MapScreenPos.y, MousePos.x, MousePos.y, MouseWorldPos.x, MouseWorldPos.y);
			lineRGBA(Ren, MousePosBox.x - 2.f, MousePosBox.y, MousePosBox.x - 2.f, MousePosBox.y + MousePosBox.h, 255, 255, 255, 255);

			float DebugToolPaneWidth = 175.f;
			if (_dbgTools.size() == 0)
			{
				auto DbgBox = FC_Draw(Font, Ren, 630.f - DebugToolPaneWidth, 15.f, "<no debug tools>");
				FC_DrawColor(Font, Ren, (DebugToolPaneWidth / 2.f), static_cast<float>(DbgBox.y - (DbgBox.h + 1)), { 255, 255, 255, 127 }, "<no debug tools>");
				FC_DrawColor(Font, Ren, (DebugToolPaneWidth / 2.f), static_cast<float>(DbgBox.y + DbgBox.h + 1), { 255, 255, 255, 127 }, "<no debug tools>");
			}
			else
			{
				ADebugTool* ThisTool = _dbgTools[_dbgToolIndex];
				ADebugTool* PrevTool = _dbgTools[_dbgToolIndex == 0 ? _dbgTools.size() - 1 : _dbgToolIndex - 1];
				ADebugTool* NextTool = _dbgTools[_dbgToolIndex == _dbgTools.size() - 1 ? 0 : _dbgToolIndex + 1];

				auto DbgBox = FC_Draw(Font, Ren, 630.f - DebugToolPaneWidth, 15.f, ThisTool->GetToolName().c_str());
				FC_DrawColor(Font, Ren, 630.f - DebugToolPaneWidth, static_cast<float>(DbgBox.y - (DbgBox.h + 1)), { 255, 255, 255, 127 }, PrevTool->GetToolName().c_str());
				auto BtmDbgBox = FC_DrawColor(Font, Ren, 630.f - DebugToolPaneWidth , static_cast<float>(DbgBox.y + DbgBox.h + 1), { 255, 255, 255, 127 }, NextTool->GetToolName().c_str());

				SDL_SetRenderDrawColor(Ren, 255, 255, 255, 255);
				SDL_RenderDrawLine(Ren, 630 - DebugToolPaneWidth, BtmDbgBox.y + BtmDbgBox.h + 3, 630, BtmDbgBox.y + BtmDbgBox.h + 3);

				auto DescBox = FC_DrawColumn(Font, Ren, 630.f - DebugToolPaneWidth, static_cast<float>(BtmDbgBox.y + BtmDbgBox.h + 5), DebugToolPaneWidth, ThisTool->GetToolDesc().c_str());

				int DescBoxY = DescBox.y + DescBox.h + 5;
				SDL_RenderDrawLine(Ren, 630 - DebugToolPaneWidth, DescBoxY - 2, 630, DescBoxY - 2);
				SDL_Rect DbgToolDrawArea = { 630.f - DebugToolPaneWidth, DescBox.y + DescBox.h + 5, DebugToolPaneWidth, 470 - DescBoxY };
				ThisTool->Render(Ren, DbgToolDrawArea);
			}
		}
		
	_screenTarget->Unbind();


	// Shade color
	// TODO: [cool] make shading match current ocean color?
	SDL_SetRenderDrawColor(Ren, 0, 0, 0, 255);
	SDL_RenderClear(Ren);

	SDL_RenderCopy(Ren, _screenTarget->Texture(), nullptr, nullptr);

	SDL_RenderPresent(Ren);
}

bool Game::HasExitBeenRequested() const
{
	return _bExitRequested;
}

Window& Game::GetWindow()
{
	return *_Window;
}

Map* Game::GetMap()
{
	return _map;
}