#include <graphics.h>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 960;

const int BUTTON_WIDTH = 192;
const int BUTTON_HEIGHT = 92;

const int OPTION_WIDTH = 900;
const int OPTION_HEIGHT = 120;

#pragma comment(lib, "MSIMG32.LIB")
#pragma comment(lib, "Winmm.lib")

DWORD GAMESTARTTIME;
bool running = true;
bool is_game_started = false;

inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

class AirWall
{
public:
	virtual ~AirWall() {};
	virtual void checkWallandObject(long& x, long& y, int height, int width) = 0;
protected:
	void checkObject(long& x, long& y, int height, int width, int obj_left, int obj_right, int obj_down, int obj_up) {
		if (x + width >= obj_left + 5 && x <= obj_right - 5) {
			if (y + height > obj_down - 20 && y + height < obj_down + 5) y = obj_down + 5 - height;
			if (y + height < obj_up + 5 && y + height > obj_up - 12) y = obj_up - 12 - height;
		}
		if (y + height < obj_down && y + height > obj_up) {
			if (x + width >= obj_left - 30 && x + width <= obj_left + 15) {
				if (x + width > obj_left - 4) x = obj_left - 4 - width;
			}
			else if (x >= obj_right - 15) {
				if (x < obj_right + 4) x = obj_right + 4;
			}
		}
	}
};

class HallWayAirWall : public AirWall
{
public:
	void checkWallandObject(long& x, long& y, int height, int width)
	{
		if (y + height < 255) y = 255 - height;
		else if (y + height > 447 && (x < 570 || x + width > 713)) y = 447 - height;
		else if (y + height >= 452) {
			if (x < 575) x = 575;
			else if (x + width > 708) x = 708 - width;
			if (y + height > 960) y = 960 - height;
		}
		if (x < 20) x = 20;
		if (x + width > 1260) x = 1260 - width;

		checkObject(x, y, height, width, 230, 435, 320, 0);//大柜子
		checkObject(x, y, height, width, 1208, 1280, 314, 0);//小柜子
		checkObject(x, y, height, width, 297, 573, 960, 393);//下面左面的东西
		checkObject(x, y, height, width, 709, 985, 960, 393);//下面右面的东西
		checkObject(x, y, height, width, 606, 606 + 66, 600 + 104, 600);//校霸（bully）
	}
};

class BasketballCourtAirWall : public AirWall
{
public:
	void checkWallandObject(long& x, long& y, int height, int width)
	{
		if (y + height < 245) y = 245 - height;
		if (y + height > 760) y = 760 - height;
		if (x < 30) x = 30;
		if (x + width > 1248) x = 1248 - width;

		checkObject(x, y, height, width, 30, 125, 530, 400);//左篮筐
		checkObject(x, y, height, width, 1154, 1220, 530, 400);//右篮筐
		checkObject(x, y, height, width, 619, 660, 525, 480);//篮球
	}
};

class CanteenAirWall : public AirWall
{
public:
	void checkWallandObject(long& x, long& y, int height, int width)
	{
		if (y + height < 605) y = 605 - height;
		if (y + height > 900) y = 900 - height;
		if (x < 190) x = 190;
		if (x + width > 1074) x = 1074 - width;

		checkObject(x, y, height, width, 266, 415, 840, 665);//左桌子
		checkObject(x, y, height, width, 500, 650, 840, 665);//中间桌子
		checkObject(x, y, height, width, 737, 885, 840, 665);//右桌子
		checkObject(x, y, height, width, 206, 245, 910, 860);//左柜子
		checkObject(x, y, height, width, 1030, 1064, 910, 845);//右柜子
	}
};

class ComputerRoomAirWall : public AirWall
{
public:
	void checkWallandObject(long& x, long& y, int height, int width)
	{
		if (y + height < 345) y = 345 - height;
		if (y + height > 845) y = 845 - height;
		if (x < 200) x = 200;
		if (x + width > 1065) x = 1065 - width;

		checkObject(x, y, height, width, 200, 296, 750, 320);//左桌
		checkObject(x, y, height, width, 970, 1070, 750, 320);//右桌
		checkObject(x, y, height, width, 505, 760, 460, 400);//上面椅子
		checkObject(x, y, height, width, 310, 382, 652, 368);//左面椅子
		checkObject(x, y, height, width, 885, 955, 652, 466);//右面椅子
	}
};

class HallAirWall : public AirWall
{
public:
	void checkWallandObject(long& x, long& y, int height, int width)
	{
		if (y + height < 230) y = 230 - height;
		if (y + height > 920) y = 920 - height;
		if (x < 188) x = 188;
		if (x + width > 1087) x = 1087 - width;

		checkObject(x, y, height, width, 204, 502, 510, 464);//一左
		checkObject(x, y, height, width, 770, 1070, 510, 464);//一右
		checkObject(x, y, height, width, 204, 502, 670, 625);//二左
		checkObject(x, y, height, width, 770, 1070, 670, 625);//二右
		checkObject(x, y, height, width, 204, 502, 835, 790);//三左
		checkObject(x, y, height, width, 770, 1070, 835, 790);//三右
		checkObject(x, y, height, width, 280, 1000, 348, 275);//舞台
	}
};

class LabAirWall : public AirWall
{
public:
	void checkWallandObject(long& x, long& y, int height, int width)
	{
		if (y + height < 361) y = 361 - height;
		if (y + height > 840) y = 840 - height;
		if (x < 140) x = 140;
		if (x + width > 1005) x = 1005 - width;

		checkObject(x, y, height, width, 135, 237, 383, 355);//左上
		checkObject(x, y, height, width, 135, 430, 555, 460);//左中
		checkObject(x, y, height, width, 260, 314, 651, 599);//左下
		checkObject(x, y, height, width, 332, 430, 746, 555);//左桌拐
		checkObject(x, y, height, width, 524, 622, 746, 430);//中桌
		checkObject(x, y, height, width, 645, 697, 649, 503);//中凳
		checkObject(x, y, height, width, 716, 814, 746, 460);//右桌
		checkObject(x, y, height, width, 836, 889, 554, 503);//右上
		checkObject(x, y, height, width, 836, 889, 649, 599);//右下
		checkObject(x, y, height, width, 168, 207, 841, 754);//灭火器
	}
};

class ClassroomAirWall : public AirWall
{
public:
	void checkWallandObject(long& x, long& y, int height, int width)
	{
		if (y + height < 251) y = 251 - height;
		if (y + height > 890) y = 890 - height;
		if (x < 260) x = 260;
		if (x + width > 980) x = 980 - width;

		checkObject(x, y, height, width, 275, 420, 330, 250);//左上桌1
		checkObject(x, y, height, width, 340, 570, 410, 340);//左上桌2
		checkObject(x, y, height, width, 600, 644, 410, 370);//左上凳
		checkObject(x, y, height, width, 586, 982, 324, 250);//右上
		checkObject(x, y, height, width, 913, 965, 569, 418);//右中
		checkObject(x, y, height, width, 420, 818, 892, 813);//下
		checkObject(x, y, height, width, 420, 495, 555, 440);//桌1
		checkObject(x, y, height, width, 577, 659, 555, 440);//桌2
		checkObject(x, y, height, width, 737, 818, 555, 440);//桌3
		checkObject(x, y, height, width, 418, 497, 717, 603);//桌4
		checkObject(x, y, height, width, 578, 657, 717, 603);//桌5
		checkObject(x, y, height, width, 737, 816, 717, 603);//桌6
	}
};

class Animation
{
public:
	Animation(LPCTSTR path, int num, int interval)
	{
		interval_ms = interval;

		TCHAR path_file[256];
		for (size_t i = 0; i < num; i++)
		{
			_stprintf_s(path_file, path, i);

			IMAGE* frame = new IMAGE();
			loadimage(frame, path_file);
			frame_list.push_back(frame);
		}
	}

	~Animation()
	{
		for (size_t i = 0; i < frame_list.size(); i++)
		{
			delete frame_list[i];
		}
	}

	void Play(int x, int y, int delta)
	{
		timer += delta;
		if (timer >= interval_ms)
		{
			idx_frame = (idx_frame + 1) % frame_list.size();
			timer = 0;
		}

		putimage_alpha(x, y, frame_list[idx_frame]);
	}

private:
	int timer = 0;
	int idx_frame = 0;
	int interval_ms = 0;
	vector<IMAGE*> frame_list;
};

class Button
{
public:
	Button(RECT rect, LPCTSTR img_idle_path, LPCTSTR img_hanging_path, LPCTSTR img_clicked_path)
	{
		region = rect;

		loadimage(&img_idle, img_idle_path);
		loadimage(&img_hanging, img_hanging_path);
		loadimage(&img_clicked, img_clicked_path);
	}

	void ProcessEvent(const ExMessage& msg) {
		switch (msg.message) {
		case WM_MOUSEMOVE:
			if (status == Status::Idle && CheckMousePosition(msg.x, msg.y)) {
				mciSendString(_T("play hanging from 0"), NULL, 0, NULL);
				status = Status::Hanging;
			}
			else if (status == Status::Hanging && !CheckMousePosition(msg.x, msg.y))
				status = Status::Idle;
			break;
		case WM_LBUTTONDOWN:
			if (CheckMousePosition(msg.x, msg.y)) {
				mciSendString(_T("play clicked from 0"), NULL, 0, NULL);
				status = Status::Clicked;
			}
			break;
		case WM_LBUTTONUP:
			if (status == Status::Clicked)
				AlreadyClicked();
			break;
		}
	}

	void Draw()
	{
		switch (status)
		{
		case Status::Idle:
			putimage_alpha(region.left, region.top, &img_idle);
			break;
		case Status::Hanging:
			putimage_alpha(region.left, region.top, &img_hanging);
			break;
		case Status::Clicked:
			putimage_alpha(region.left, region.top, &img_clicked);
			break;
		}
	}

protected:
	virtual void AlreadyClicked() = 0;

private:
	enum class Status
	{
		Idle = 0,
		Hanging,
		Clicked
	};

	RECT region;
	IMAGE img_idle;
	IMAGE img_hanging;
	IMAGE img_clicked;
	Status status = Status::Idle;

	bool CheckMousePosition(int x, int y) {
		return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
	}
};

class StartGameButton : public Button
{
public:
	StartGameButton(RECT rect, LPCTSTR img_idle_path, LPCTSTR img_hanging_path, LPCTSTR img_clicked_path)
		: Button(rect, img_idle_path, img_hanging_path, img_clicked_path) {
	}

private:
	void AlreadyClicked()
	{
		is_game_started = true;

		mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);

		GAMESTARTTIME = GetTickCount();
	}
};

class ExitGameButton : public Button
{
public:
	ExitGameButton(RECT rect, LPCTSTR img_idle_path, LPCTSTR img_hanging_path, LPCTSTR img_clicked_path)
		: Button(rect, img_idle_path, img_hanging_path, img_clicked_path) {
	}

private:
	void AlreadyClicked()
	{
		running = false;
	}
};

class Player
{
public:
	Player()
	{
		loadimage(&img_shadow, _T("pic/shadow_player.png"));
		anim_left = new Animation(_T("pic/player_left_%d.png"), 8, 45);
		anim_right = new Animation(_T("pic/player_right_%d.png"), 8, 45);
	}

	~Player()
	{
		delete anim_left;
		delete anim_right;
	}

	void ProcessEvent(const ExMessage& msg)
	{
		if (return_choose != 'O') return_choose = 'O';
		if (msg.message == WM_KEYDOWN)
		{
			switch (msg.vkcode)
			{
			case 'w':
			case 'W':
			case VK_UP:
				is_move_up = true;
				break;
			case 's':
			case 'S':
			case VK_DOWN:
				is_move_down = true;
				break;
			case 'a':
			case 'A':
			case VK_LEFT:
				is_move_left = true;
				break;
			case 'd':
			case 'D':
			case VK_RIGHT:
				is_move_right = true;
				break;
			case 'e':
			case 'E':
				is_e_down = true;
				break;
			case 'r':
			case 'R':
				is_r_down = true;
				break;
			}
		}
		else if (msg.message == WM_KEYUP)
		{
			switch (msg.vkcode)
			{
			case 'w':
			case 'W':
			case VK_UP:
				is_move_up = false;
				break;
			case 's':
			case 'S':
			case VK_DOWN:
				is_move_down = false;
				break;
			case 'a':
			case 'A':
			case VK_LEFT:
				is_move_left = false;
				break;
			case 'd':
			case 'D':
			case VK_RIGHT:
				is_move_right = false;
				break;
			case 'e':
			case 'E':
				is_e_down = false;
				break;
			case 'r':
			case 'R':
				is_r_down = false;
				break;
			}
		}
		else if (msg.message == WM_LBUTTONDOWN) {
			int x = msg.x, y = msg.y;
			if (x >= 209 && x <= 209 + OPTION_WIDTH && y >= 238 && y <= 238 + OPTION_HEIGHT) return_choose = 'A';
			else if (x >= 209 && x <= 209 + OPTION_WIDTH && y >= 438 && y <= 438 + OPTION_HEIGHT) return_choose = 'B';
			else if (x >= 209 && x <= 209 + OPTION_WIDTH && y >= 634 && y <= 634 + OPTION_HEIGHT) return_choose = 'C';
			else if (x >= 209 && x <= 209 + OPTION_WIDTH && y >= 828 && y <= 828 + OPTION_HEIGHT) return_choose = 'D';
		}
	}

	void Move(AirWall* airwall)
	{
		int dir_x = is_move_right - is_move_left;
		int dir_y = is_move_down - is_move_up;
		double dir_len = sqrt(dir_x * dir_x + dir_y * dir_y);
		if (dir_len != 0)
		{
			double norm_x = dir_x / dir_len;
			double norm_y = dir_y / dir_len;
			position.x += (int)(speed * norm_x);
			position.y += (int)(speed * norm_y);
		}

		//边界检测
		airwall->checkWallandObject(position.x, position.y, PLAYER_HEIGHT, PLAYER_WIDTH);
	}

	void Draw(int delta)
	{
		int pos_shadow_x = position.x + (PLAYER_WIDTH / 2 - SHADOW_WIDTH / 2);
		int pos_shadow_y = position.y + PLAYER_HEIGHT - 16;
		putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

		static bool facing_left = false;
		int dir_x = is_move_right - is_move_left;
		if (dir_x < 0)
			facing_left = true;
		else if (dir_x > 0)
			facing_left = false;

		if (facing_left)
			anim_left->Play(position.x, position.y, delta);
		else
			anim_right->Play(position.x, position.y, delta);
	}

	int getX() { return position.x; }
	int getY() { return position.y; }
	int getSpeed() { return speed; }
	bool Check_is_e_down() { return is_e_down; }
	bool Check_is_r_down() { return is_r_down; }
	char Check_click_which() { return return_choose; }
private:
	const int PLAYER_WIDTH = 48;
	const int PLAYER_HEIGHT = 84;
	const int SHADOW_WIDTH = 48;

	int speed = 5;
	void EffectChangeSpeed(int change_speed_to) { speed = change_speed_to; }

	IMAGE img_shadow;
	Animation* anim_left;
	Animation* anim_right;
	POINT position = { 30, 269 };
	bool is_move_up = false;
	bool is_move_down = false;
	bool is_move_left = false;
	bool is_move_right = false;
	bool is_e_down = false;
	bool is_r_down = false;
	char return_choose = 'O';

	friend class SceneHallway;
	friend class SceneBasketballCourt;
	friend class SceneCanteen;
	friend class SceneHall;
	friend class SceneComputerRoom;
	friend class SceneLab;
	void SceneChange_set_player_pos(int x = 0, int y = 0)
	{
		position.x = x;
		position.y = y;
	}
};

class Npc
{
public:
	Npc(int npc_width, int npc_height, int shadow_width) : NPC_WIDTH(npc_width), NPC_HEIGHT(npc_height)
		, SHADOW_WIDTH(shadow_width), anim_front(NULL) {
	}
	virtual ~Npc() {};
	virtual void Draw(int delta) = 0;
	//virtual void Move() = 0;
protected:
	const int SPEED = 5;
	const int NPC_WIDTH;
	const int NPC_HEIGHT;
	const int SHADOW_WIDTH;

	IMAGE img_shadow;
	Animation* anim_front;
	POINT position = { 0, 0 };
};

class Bully : public Npc
{
public:
	Bully() : Npc(66, 104, 66)
	{
		loadimage(&img_shadow, _T("pic/shadow_bully.png"));
		anim_front = new Animation(_T("pic/bully_front_%d.png"), 3, 250);
		position.x = 606;
		position.y = 600;
	}

	~Bully()
	{
		delete anim_front;
	}

	void Draw(int delta)
	{
		int pos_shadow_x = position.x + (NPC_WIDTH / 2 - SHADOW_WIDTH / 2);
		int pos_shadow_y = position.y + NPC_HEIGHT - 25;
		putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

		anim_front->Play(position.x, position.y, delta);
	}
};

class MoveObject
{
public:
	virtual void Draw(int delta)
	{
		anim_front->Play(position.x, position.y, delta);
	}
protected:
	Animation* anim_front;
	POINT position = { 0, 0 };
};

class Cola_far_from_player : public MoveObject
{
public:
	Cola_far_from_player()
	{
		anim_front = new Animation(_T("pic/cola_%d.png"), 1, 500);
		position.x = 1203;
		position.y = 237;
	}
	~Cola_far_from_player()
	{
		delete anim_front;
	}
};

class Cola_close_to_player : public MoveObject
{
public:
	Cola_close_to_player()
	{
		anim_front = new Animation(_T("pic/cola_%d.png"), 2, 500);
		position.x = 1203;
		position.y = 237;
	}
	~Cola_close_to_player()
	{
		delete anim_front;
	}
};

class Button_E_short : public MoveObject
{
public:
	Button_E_short(int x = 0, int y = 0)
	{
		anim_front = new Animation(_T("pic/button_e_%d.png"), 2, 500);
		position.x = x;
		position.y = y;
	}
	~Button_E_short()
	{
		delete anim_front;
	}
};

class Button_E_long : public MoveObject
{
public:
	Button_E_long(int x = 0, int y = 0)
	{
		anim_front = new Animation(_T("pic/button_e_%d.png"), 3, 500);
		position.x = x;
		position.y = y;
	}
	~Button_E_long()
	{
		delete anim_front;
	}
};

class Button_R_short : public MoveObject
{
public:
	Button_R_short(int x = 0, int y = 0)
	{
		anim_front = new Animation(_T("pic/button_r_%d.png"), 2, 500);
		position.x = x;
		position.y = y;
	}
	~Button_R_short()
	{
		delete anim_front;
	}
	void SetPosition(int x, int y) {
		position.x = x;
		position.y = y;
	}
};

class HallwayBullyDialog : public MoveObject
{
public:
	HallwayBullyDialog()
	{
		anim_front = new Animation(_T("pic/HallWayBullyDialog_%d.png"), 1, 500);
		position.x = 676;
		position.y = 580;
	}
	~HallwayBullyDialog()
	{
		delete anim_front;
	}
};

class Basketball : public MoveObject
{
public:
	Basketball()
	{
		anim_front = new Animation(_T("pic/basketball_%d.png"), 1, 500);
		position.x = 597;
		position.y = 455;
	}
	~Basketball()
	{
		delete anim_front;
	}

	void Move(int& delta)
	{
		static int x_left_time = -1;
		static int x_max_time = -1;
		if (delta > 0) {
			x_left_time = delta;
			x_max_time = delta;
			delta = 0;
		}
		else if (delta == -2) {
			position.x = 597;
			position.y = 455;
		}
		if (x_left_time >= 0) {
			position.x = 597 - (x_max_time - x_left_time) * 5;
			if (x_left_time > x_max_time / 2) {
				position.y = 455 - (int)(sqrt((x_max_time - x_left_time) * 5) * 10);
			}
			else {
				position.y = 455 - (int)(sqrt(x_left_time * 5) * 10);
			}
			if (x_left_time == 0) {
				mciSendString(_T("play wrong from 0"), NULL, 0, NULL);
				delta = -1;
			}
			x_left_time--;
		}
	}

	int GetBallHeartX() { return position.x + 40; }
	int GetBallHeartY() { return position.y + 40; }

private:
	const int SPEED = 5;
};

class LeftDownArrow : public MoveObject
{
public:
	LeftDownArrow(int x = 0, int y = 0)
	{
		anim_front = new Animation(_T("pic/arrow_%d.png"), 4, 150);
		position.x = x;
		position.y = y;
	}

	~LeftDownArrow()
	{
		delete anim_front;
	}

	int getX() { return position.x; }
	int getY() { return position.y; }
};

class WaterOnTheFloor : public MoveObject
{
public:
	WaterOnTheFloor()
	{
		anim_front = new Animation(_T("pic/water_on_the_floor_%d.png"), 1, 500);
		position.x = 523;
		position.y = 605;
	}
	~WaterOnTheFloor()
	{
		delete anim_front;
	}
};

class WaterOnThePlayer : public MoveObject
{
public:
	WaterOnThePlayer()
	{
		anim_front = new Animation(_T("pic/water_on_the_player_%d.png"), 2, 500);
		position.x = 0;
		position.y = 0;
	}

	~WaterOnThePlayer()
	{
		delete anim_front;
	}

	void set_position(int x, int y)
	{
		position.x = x;
		position.y = y;
	}
};

class PutOutFire : public MoveObject
{
public:
	PutOutFire()
	{
		loadimage(&img_shadow, _T("pic/shadow_put_out_fire.png"));
		anim_front = new Animation(_T("pic/put_out_fire_%d.png"), 1, 500);
		position.x = 168;
		position.y = 754;
	}

	~PutOutFire()
	{
		delete anim_front;
	}

	void Draw(int delta)
	{
		int pos_shadow_x = position.x;
		int pos_shadow_y = position.y + 52;
		putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);
		anim_front->Play(position.x, position.y, delta);
	}

private:
	IMAGE img_shadow;
};

class Fire : public MoveObject
{
public:
	Fire(int x = 0, int y = 0)
	{
		anim_front = new Animation(_T("pic/fire_%d.png"), 6, 150);
		position.x = x;
		position.y = y;
	}

	~Fire()
	{
		delete anim_front;
	}

	int getX() { return position.x; }
	int getY() { return position.y; }
};

class Phone : public MoveObject
{
public:
	Phone()
	{
		anim_front = new Animation(_T("pic/phone_%d.png"), 1, 500);
		position.x = 355;
		position.y = 345;
	}

	~Phone()
	{
		delete anim_front;
	}

};

class Questions {
public:
	Questions(char ans) : answer(ans) {};

	virtual ~Questions() = default;

	virtual void Draw()
	{
		putimage(position.x, position.y, &img_question);
	}

	char GetAnswer() { return answer; }
protected:
	IMAGE img_question;
	POINT position = { 0, 0 };
	const char answer;
};

class HallwayQuestion : public Questions
{
public:
	HallwayQuestion() : Questions('A')
	{
		loadimage(&img_question, _T("pic/hallway_question.png"));
	}

};

class BasketballCourtQuestion : public Questions
{
public:
	BasketballCourtQuestion() : Questions('A')
	{
		loadimage(&img_question, _T("pic/basketball_court_question.png"));
	}

};

class CanteenQuestion : public Questions
{
public:
	CanteenQuestion() : Questions('C')
	{
		loadimage(&img_question, _T("pic/canteen_question.png"));
	}

};

class HallQuestion : public Questions
{
public:
	HallQuestion() : Questions('A')
	{
		loadimage(&img_question, _T("pic/hall_question.png"));
	}

};

class ComputerRoomQuestion : public Questions
{
public:
	ComputerRoomQuestion() : Questions('D')
	{
		loadimage(&img_question, _T("pic/computer_room_question.png"));
	}

};

class DoNotMoveObject
{
public:
	virtual void Draw()
	{
		putimage(position.x, position.y, &img_object);
	}
protected:
	IMAGE img_object;
	POINT position = { 0, 0 };
};

class SignInSuccess : public DoNotMoveObject
{
public:
	SignInSuccess()
	{
		loadimage(&img_object, _T("pic/sign_in_success.png"));
		position.x = 0;
		position.y = 0;
	}

};

class SignInLate : public DoNotMoveObject
{
public:
	SignInLate()
	{
		loadimage(&img_object, _T("pic/sign_in_late.png"));
		position.x = 0;
		position.y = 0;
	}

};

class Scene
{
public:
	virtual ~Scene() {}
	virtual void Draw(int delta) = 0;
	virtual void checkSceneChange(Scene*& nowscene) = 0;
	AirWall* airwall;
	static Player player;
	static void PrintNowTime() {
		DWORD nowTime = GetTickCount();
		DWORD delta = (nowTime - GAMESTARTTIME) / 1000;
		int BeiJingTime = 28720 + delta;
		static TCHAR str[64];
		settextstyle(30, 15, _T("宋体"));
		if (delta >= 80) settextcolor(RED);
		_stprintf_s(str, _T("现在时间：%02d:%02d:%02d"), BeiJingTime / 3600, BeiJingTime % 3600 / 60, BeiJingTime % 60);
		outtextxy(0, 0, str);
	}
protected:
	IMAGE img_background;
};

Player Scene::player;
vector<Scene*> all_scene_vector;

class SceneHallway : public Scene
{
public:
	SceneHallway()
		: cola_button_e_short(1210, 200), bully_button_e_short(606 + 66 + 75, 600 - 75), door_button_e_short(625, 100)
	{
		loadimage(&img_background, _T("pic/Hallway.png"));
		airwall = new HallWayAirWall();
	}

	~SceneHallway()
	{
		delete airwall;
	}

	void Draw(int delta)
	{
		putimage(0, 0, &img_background);
		if (is_cola_alive)
		{
			if (fabs(1203 - player.getX()) <= 100 && (player.getY() - 237) <= 50) {
				close_cola.Draw(1000 / 90);
				cola_button_e_short.Draw(1000 / 90);
				if (player.Check_is_e_down()) {
					mciSendString(_T("play right from 50"), NULL, 0, NULL);
					is_cola_alive = false;
				}
			}
			else {
				far_cola.Draw(1000 / 90);
			}
		}
		player.Draw(1000 / 90);
		bully.Draw(1000 / 90);
		if (!is_bully_give_key && fabs(606 - player.getX()) <= 50 && fabs(player.getY() - 600) <= 100) {
			hall_way_bully_dialog.Draw(1000 / 90);
			if (!is_cola_alive) {
				bully_button_e_short.Draw(1000 / 90);
				if (player.Check_is_e_down()) {
					mciSendString(_T("play right from 50"), NULL, 0, NULL);
					is_bully_give_key = true;
				}
			}
		}
		if (is_bully_give_key && !door_e_already_down && fabs(644 - player.getX()) <= 50 && fabs(player.getY() - 177) <= 50) {
			door_button_e_short.Draw(1000 / 90);
			if (player.Check_is_e_down()) {
				door_e_already_down = true;
			}
		}

		if (door_e_already_down && !is_success) {
			hallway_question.Draw();
			if (player.Check_click_which() != 'O' && player.Check_click_which() != hallway_question.GetAnswer()) {
				mciSendString(_T("play wrong from 0"), NULL, 0, NULL);
				player.EffectChangeSpeed(1);//友元类实现修改（不安全）
				is_success = true;
			}
			else if (player.Check_click_which() != 'O') {
				mciSendString(_T("play right from 50"), NULL, 0, NULL);
				is_success = true;
			}

		}
	}

	void checkSceneChange(Scene*& nowscene)
	{
		if (is_success) // 到达指定地点
		{
			//直接改变指针指向
			nowscene = all_scene_vector[1];
			player.SceneChange_set_player_pos(0, 0);// 友元类实现坐标修改（不安全）
		}
	}

private:
	bool is_cola_alive = true;
	bool is_bully_give_key = false;
	bool is_success = false;
	bool door_e_already_down = false;
	Bully bully;
	Cola_far_from_player far_cola;
	Cola_close_to_player close_cola;
	Button_E_short cola_button_e_short;
	Button_E_short bully_button_e_short;
	Button_E_short door_button_e_short;
	HallwayBullyDialog hall_way_bully_dialog;
	HallwayQuestion hallway_question;
	//结束时改变NowScene指针指向
};

class SceneBasketballCourt : public Scene
{
public:
	SceneBasketballCourt()
		: basketball_button_e_long(623, 430), retry_button_r_short(747, 525), basketball_arrow(130, 390),
		is_success_leave_arrow(60, 700)
	{
		loadimage(&img_background, _T("pic/BasketballCourt.png"));
		airwall = new BasketballCourtAirWall();
	}

	~SceneBasketballCourt()
	{
		delete airwall;
	}

	void Draw(int delta)
	{
		if (!is_get_begin_time) {
			scene_begin_time = GetTickCount();
			is_get_begin_time = true;
		}
		if (!is_check_speed && player.getSpeed() == 1) {
			is_lower_speed = true;
			is_check_speed = true;
		}

		putimage(0, 0, &img_background);
		if (!is_ball_in) basketball.Draw(1000 / 90);
		player.Draw(1000 / 90);

		if (is_lower_speed) {
			water_on_the_player.set_position(player.getX(), player.getY());
			water_on_the_player.Draw(1000 / 90);
			if (GetTickCount() - scene_begin_time >= 5000) {
				is_lower_speed = false;
				player.EffectChangeSpeed(5);
			}
		}

		if (!is_ball_in && fabs(597 - player.getX()) <= 50 && fabs(player.getY() - 455) <= 100) {
			static int delta = 0;
			static DWORD start_time = 0;
			static DWORD end_time = 0;
			static bool e_already_down = false;

			if (!e_already_down)
			{
				basketball_button_e_long.Draw(1000 / 90);
				basketball_arrow.Draw(1000 / 90);
			}

			if (player.Check_is_e_down() && !e_already_down)
			{
				//画虚线方向；
				//改篮球坐标
				start_time = GetTickCount();
				e_already_down = true;
			}

			if (!player.Check_is_e_down() && start_time != 0)
			{
				end_time = GetTickCount();
				delta = end_time - start_time;
				delta /= 10;
				start_time = 0;
			}

			basketball.Move(delta);

			if (fabs(110 - basketball.GetBallHeartX()) <= 10 && fabs(basketball.GetBallHeartY() - 440) <= 10) {
				mciSendString(_T("play right from 50"), NULL, 0, NULL);
				is_ball_in = true;
			}

			if (delta == -1 && e_already_down)
			{
				retry_button_r_short.Draw(1000 / 90);
				if (player.Check_is_r_down())
				{
					int tmp = -2;
					delta = 0;
					start_time = 0;
					end_time = 0;
					e_already_down = false;
					basketball.Move(tmp);
				}
			}
		}

		if (is_ball_in && !(player.getX() <= 50 && player.getY() >= 670)) {
			is_success_leave_arrow.Draw(1000 / 90);
		}
		else if (is_ball_in) {
			basketball_court_question.Draw();
			if (player.Check_click_which() != 'O' && player.Check_click_which() != basketball_court_question.GetAnswer()) {
				mciSendString(_T("play wrong from 0"), NULL, 0, NULL);
				player.EffectChangeSpeed(1);//友元类实现修改（不安全）
				is_success = true;
			}
			else if (player.Check_click_which() != 'O') {
				mciSendString(_T("play right from 50"), NULL, 0, NULL);
				is_success = true;
			}

		}
	}

	void checkSceneChange(Scene*& nowscene)
	{
		if (is_success) // 到达指定地点
		{
			//直接改变指针指向
			nowscene = all_scene_vector[2];
			player.SceneChange_set_player_pos(1000, 560);// 友元类实现坐标修改（不安全）
		}
	}
private:
	bool is_ball_in = false;
	bool is_lower_speed = false;
	bool is_success = false;
	bool is_get_begin_time = false;
	bool is_check_speed = false;
	Button_E_long basketball_button_e_long;
	Button_R_short retry_button_r_short;
	Basketball basketball;
	LeftDownArrow basketball_arrow;
	LeftDownArrow is_success_leave_arrow;
	DWORD scene_begin_time = 0;
	WaterOnThePlayer water_on_the_player;
	BasketballCourtQuestion basketball_court_question;
	//结束时改变NowScene指针指向
};

class SceneCanteen : public Scene
{
public:
	SceneCanteen() : where_to_go_arrow(205, 570)
	{
		loadimage(&img_background, _T("pic/Canteen.png"));
		airwall = new CanteenAirWall();
	}

	~SceneCanteen()
	{
		delete airwall;
	}

	void Draw(int delta)
	{
		if (!is_get_begin_time) {
			scene_begin_time = GetTickCount();
			is_get_begin_time = true;
		}
		if (!is_check_speed && player.getSpeed() == 1) {
			is_lower_speed = true;
			is_check_speed = true;
		}

		putimage(0, 0, &img_background);
		water_on_the_floor.Draw(1000 / 90);
		player.Draw(1000 / 90);
		where_to_go_arrow.Draw(1000 / 90);

		if (!is_touch_water && player.getX() >= 523 && player.getX() <= 630 && player.getY() <= 605) {
			mciSendString(_T("play wrong from 0"), NULL, 0, NULL);
			is_touch_water = true;
			touch_water_begin_time = GetTickCount();
		}

		static bool already_change_speed = false;
		if (!already_change_speed && is_touch_water) {
			player.EffectChangeSpeed(1);//友元类实现修改（不安全）
		}

		if (is_lower_speed && !is_touch_water) {
			water_on_the_player.set_position(player.getX(), player.getY());
			water_on_the_player.Draw(1000 / 90);
			if (GetTickCount() - scene_begin_time >= 5000) {
				is_lower_speed = false;
				player.EffectChangeSpeed(5);
			}
		}

		if (is_touch_water)
		{
			water_on_the_player.set_position(player.getX(), player.getY());
			water_on_the_player.Draw(1000 / 90);
			if (GetTickCount() - touch_water_begin_time >= 5000) {
				is_touch_water = false;
				player.EffectChangeSpeed(5);
			}
		}

		if (player.getY() <= 560 && player.getX() <= 200) {
			canteen_question.Draw();
			if (player.Check_click_which() != 'O' && player.Check_click_which() != canteen_question.GetAnswer()) {
				mciSendString(_T("play wrong from 0"), NULL, 0, NULL);
				player.EffectChangeSpeed(1);//友元类实现修改（不安全）
				is_success = true;
			}
			else if (player.Check_click_which() != 'O') {
				mciSendString(_T("play right from 50"), NULL, 0, NULL);
				is_success = true;
			}
		}
	}
	void checkSceneChange(Scene*& nowscene)
	{
		if (is_success) // 到达指定地点
		{
			//直接改变指针指向
			nowscene = all_scene_vector[3];
			player.SceneChange_set_player_pos(600, 820);// 友元类实现坐标修改（不安全）
		}
	}

private:
	LeftDownArrow where_to_go_arrow;
	WaterOnTheFloor water_on_the_floor;
	WaterOnThePlayer water_on_the_player;
	DWORD scene_begin_time = 0;
	DWORD touch_water_begin_time = 0;
	CanteenQuestion canteen_question;
	bool is_touch_water = false;
	bool is_lower_speed = false;
	bool is_success = false;
	bool is_get_begin_time = false;
	bool is_check_speed = false;
	//结束时改变NowScene指针指向
};

class SceneHall : public Scene
{
public:
	SceneHall() : where_to_go_arrow{ LeftDownArrow(780, 200), LeftDownArrow(540, 220), LeftDownArrow(805, 210), LeftDownArrow(475, 190),
	LeftDownArrow(780, 200), LeftDownArrow(625, 185), LeftDownArrow(505, 215), LeftDownArrow(400, 200), LeftDownArrow(890, 170),
	LeftDownArrow(650, 220), LeftDownArrow(190, 330) }
	{
		loadimage(&img_background, _T("pic/Hall.png"));
		airwall = new HallAirWall();
	}

	~SceneHall()
	{
		delete airwall;
	}

	void Draw(int delta)
	{
		if (!is_get_begin_time) {
			scene_begin_time = GetTickCount();
			is_get_begin_time = true;
		}
		if (!is_check_speed && player.getSpeed() == 1) {
			is_lower_speed = true;
			is_check_speed = true;
		}

		putimage(0, 0, &img_background);
		player.Draw(1000 / 90);
		static int arrow_index = 0;
		if (arrow_index < 10) where_to_go_arrow[arrow_index].Draw(1000 / 90);

		if (is_lower_speed) {
			water_on_the_player.set_position(player.getX(), player.getY());
			water_on_the_player.Draw(1000 / 90);
			if (GetTickCount() - scene_begin_time >= 5000) {
				is_lower_speed = false;
				player.EffectChangeSpeed(5);
			}
		}

		if (arrow_index < 10 && fabs(where_to_go_arrow[arrow_index].getX() - player.getX()) <= 50
			&& fabs(player.getY() - where_to_go_arrow[arrow_index].getY()) <= 50) {
			mciSendString(_T("play right from 50"), NULL, 0, NULL);
			arrow_index++;
		}

		if (arrow_index == 10) {
			where_to_go_arrow[arrow_index].Draw(1000 / 90);
		}

		if (fabs(where_to_go_arrow[10].getX() - player.getX()) <= 50
			&& fabs(player.getY() - where_to_go_arrow[10].getY()) <= 50) {
			hall_question.Draw();
			if (player.Check_click_which() != 'O' && player.Check_click_which() != hall_question.GetAnswer()) {
				mciSendString(_T("play wrong from 0"), NULL, 0, NULL);
				player.EffectChangeSpeed(1);//友元类实现修改（不安全）
				is_success = true;
			}
			else if (player.Check_click_which() != 'O') {
				mciSendString(_T("play right from 50"), NULL, 0, NULL);
				is_success = true;
			}
		}
	}
	void checkSceneChange(Scene*& nowscene)
	{
		if (is_success)  // 到达指定地点
		{
			//直接改变指针指向
			nowscene = all_scene_vector[4];
			player.SceneChange_set_player_pos(970, 720);// 友元类实现坐标修改（不安全）
		}
	}

private:
	bool is_success = false;
	bool is_lower_speed = false;
	bool is_get_begin_time = false;
	bool is_check_speed = false;
	DWORD scene_begin_time = 0;
	WaterOnThePlayer water_on_the_player;
	LeftDownArrow where_to_go_arrow[11];
	HallQuestion hall_question;
	//结束时改变NowScene指针指向
};

class SceneComputerRoom : public Scene
{
public:
	SceneComputerRoom() : where_to_go_arrow(655, 210), computer_e_button_short(620, 270), where_to_go_arrow_success(200, 790)
	{
		loadimage(&img_background, _T("pic/ComputerRoom.png"));
		airwall = new ComputerRoomAirWall();
	}

	~SceneComputerRoom()
	{
		delete airwall;
	}

	void Draw(int delta)
	{
		if (!is_get_begin_time) {
			scene_begin_time = GetTickCount();
			is_get_begin_time = true;
		}
		if (!is_check_speed && player.getSpeed() == 1) {
			is_lower_speed = true;
			is_check_speed = true;
		}

		putimage(0, 0, &img_background);
		player.Draw(1000 / 90);

		if (is_lower_speed && !is_wrong) {
			water_on_the_player.set_position(player.getX(), player.getY());
			water_on_the_player.Draw(1000 / 90);
			if (GetTickCount() - scene_begin_time >= 5000) {
				is_lower_speed = false;
				player.EffectChangeSpeed(5);
			}
		}

		if (!is_success && !e_already_down) {
			where_to_go_arrow.Draw(1000 / 90);
			if (fabs(620 - player.getX()) <= 50 && fabs(player.getY() - 270) <= 50) {
				computer_e_button_short.Draw(1000 / 90);
				if (player.Check_is_e_down()) {
					e_already_down = true;
				}
			}
		}

		if (e_already_down && !is_success) {
			computer_room_question.Draw();
			if (player.Check_click_which() != 'O' && player.Check_click_which() != computer_room_question.GetAnswer()) {
				mciSendString(_T("play wrong from 0"), NULL, 0, NULL);
				player.EffectChangeSpeed(1);//友元类实现修改（不安全）
				is_wrong = true;
				is_success = true;
				wrong_begin_time = GetTickCount();
			}
			else if (player.Check_click_which() != 'O') {
				mciSendString(_T("play right from 50"), NULL, 0, NULL);
				is_success = true;
			}
		}

		if (is_wrong) {
			water_on_the_player.set_position(player.getX(), player.getY());
			water_on_the_player.Draw(1000 / 90);
			if (GetTickCount() - wrong_begin_time >= 5000) {
				is_wrong = false;
				player.EffectChangeSpeed(5);
			}
		}

		if (is_success) {
			where_to_go_arrow_success.Draw(1000 / 90);
		}
	}
	void checkSceneChange(Scene*& nowscene)
	{
		if (is_success && player.getX() <= 210 && player.getY() >= 740)  // 到达指定地点
		{
			//直接改变指针指向
			mciSendString(_T("play right from 50"), NULL, 0, NULL);
			nowscene = all_scene_vector[5];
			player.SceneChange_set_player_pos(930, 500);// 友元类实现坐标修改（不安全）
			if (is_wrong) player.EffectChangeSpeed(5);//友元类实现修改（不安全）

		}
	}

private:
	LeftDownArrow where_to_go_arrow;
	LeftDownArrow where_to_go_arrow_success;
	ComputerRoomQuestion computer_room_question;
	Button_E_short computer_e_button_short;
	WaterOnThePlayer water_on_the_player;
	DWORD scene_begin_time = 0;
	DWORD wrong_begin_time = 0;
	bool is_lower_speed = false;
	bool e_already_down = false;
	bool is_success = false;
	bool is_wrong = false;
	bool is_get_begin_time = false;
	bool is_check_speed = false;
	//结束时改变NowScene指针指向
};

class SceneLab : public Scene
{
public:
	SceneLab() : put_out_fire_e_button_short(173, 765), fires_in_lab{ Fire(588, 265), Fire(865, 265),
	Fire(364, 591), Fire(550, 575), Fire(550, 479), Fire(747, 488), Fire(744, 598) }, where_to_go_arrow_success(150, 340)
	{
		loadimage(&img_background, _T("pic/Lab.png"));
		airwall = new LabAirWall();
	}

	~SceneLab()
	{
		delete airwall;
	}

	void Draw(int delta)
	{
		putimage(0, 0, &img_background);
		player.Draw(1000 / 90);
		if (is_put_out_fire_alive) put_out_fire.Draw(1000 / 90);
		if (is_put_out_fire_alive && fabs(168 - player.getX()) <= 50 && fabs(player.getY() - 754) <= 50) {
			put_out_fire_e_button_short.Draw(1000 / 90);
			if (player.Check_is_e_down()) {
				mciSendString(_T("play right from 50"), NULL, 0, NULL);
				is_put_out_fire_alive = false;
			}
		}

		for (int i = 0; i < 7; i++) {
			if (is_fires_in_lab_alive[i]) {
				fires_in_lab[i].Draw(1000 / 90);
				if (!is_put_out_fire_alive && fabs(fires_in_lab[i].getX() - player.getX()) <= 80 && fabs(player.getY() - fires_in_lab[i].getY()) <= 80) {
					fire_button_r_short.SetPosition(fires_in_lab[i].getX(), fires_in_lab[i].getY());
					fire_button_r_short.Draw(1000 / 90);
					if (player.Check_is_r_down()) {
						mciSendString(_T("play right from 50"), NULL, 0, NULL);
						is_fires_in_lab_alive[i] = false;
						how_many_fires_left--;
					}
				}
			}
		}

		if (!is_success && !how_many_fires_left) {
			is_success = true;
		}

		if (is_success) {
			where_to_go_arrow_success.Draw(1000 / 90);
		}
	}
	void checkSceneChange(Scene*& nowscene)
	{
		if (is_success && player.getX() <= 148 && player.getY() <= 380)  // 到达指定地点
		{
			mciSendString(_T("play right from 50"), NULL, 0, NULL);
			nowscene = all_scene_vector[6];
			player.SceneChange_set_player_pos(920, 570);
		}
	}

private:
	PutOutFire put_out_fire;
	Button_E_short put_out_fire_e_button_short;
	Button_R_short fire_button_r_short;
	Fire fires_in_lab[7];
	LeftDownArrow where_to_go_arrow_success;
	int how_many_fires_left = 7;
	bool is_fires_in_lab_alive[7] = { true, true, true, true, true, true, true };
	bool is_put_out_fire_alive = true;
	bool is_success = false;
	//结束时改变NowScene指针指向
};

class SceneClassroom : public Scene
{
public:
	SceneClassroom() : phone_button_e_short(355, 345)
	{
		loadimage(&img_background, _T("pic/Classroom.png"));
		airwall = new ClassroomAirWall();
	}

	~SceneClassroom()
	{
		delete airwall;
	}

	void Draw(int delta)
	{
		if (!is_success) {
			putimage(0, 0, &img_background);
			player.Draw(1000 / 90);
		}

		if (!is_get_phone) {
			phone.Draw(1000 / 90);
			if (fabs(355 - player.getX()) <= 50 && fabs(player.getY() - 345) <= 50) {
				phone_button_e_short.Draw(1000 / 90);
				if (player.Check_is_e_down()) is_get_phone = true;
			}
		}
		if (is_get_phone) {
			complete_time = GetTickCount() - GAMESTARTTIME;
			if (complete_time >= 80000) {
				sign_in_late.Draw();
				mciSendString(_T("play loss from 0"), NULL, 0, NULL);
			}
			else {
				is_success = true;
				sign_in_success.Draw();
				mciSendString(_T("play win from 0"), NULL, 0, NULL);
			}
			FlushBatchDraw();
			Sleep(500);
			is_draw = true;
		}
	}
	void checkSceneChange(Scene*& nowscene)
	{
		if (is_get_phone && is_draw)
		{
			TCHAR text[256];
			if (is_success) _stprintf_s(text, _T("签到成功！\n用时%d秒"), complete_time / 1000);
			else _stprintf_s(text, _T("你迟到了！平时分 -1 ！\n用时%d秒"), complete_time / 1000);
			MessageBox(GetHWnd(), text, _T("游戏结束"), MB_OK);
			running = false;
		}
	}

private:
	Phone phone;
	Button_E_short phone_button_e_short;
	SignInSuccess sign_in_success;
	SignInLate sign_in_late;
	int complete_time = 0;
	bool is_get_phone = false;
	bool is_success = false;
	bool is_draw = false;
	//结束时改变NowScene指针指向
};

void AddSceneTo_all_scene_vector()
{
	Scene* tmp_scene = NULL;
	tmp_scene = new SceneHallway();
	all_scene_vector.push_back(tmp_scene);
	tmp_scene = new SceneBasketballCourt();
	all_scene_vector.push_back(tmp_scene);
	tmp_scene = new SceneCanteen();
	all_scene_vector.push_back(tmp_scene);
	tmp_scene = new SceneHall();
	all_scene_vector.push_back(tmp_scene);
	tmp_scene = new SceneComputerRoom();
	all_scene_vector.push_back(tmp_scene);
	tmp_scene = new SceneLab();
	all_scene_vector.push_back(tmp_scene);
	tmp_scene = new SceneClassroom();
	all_scene_vector.push_back(tmp_scene);
	//添加场景
}

void Delete_all_scene_vector()
{
	for (auto& u : all_scene_vector)
	{
		delete u;
	}
}

int main()
{
	initgraph(1280, 960);

	ExMessage msg;
	AddSceneTo_all_scene_vector();
	Scene* NowScene = all_scene_vector[0]; //改场景
	IMAGE img_menu;

	RECT region_start_button, region_exit_button;

	region_start_button.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_start_button.right = region_start_button.left + BUTTON_WIDTH;
	region_start_button.top = 530;
	region_start_button.bottom = region_start_button.top + BUTTON_HEIGHT;

	region_exit_button.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_exit_button.right = region_exit_button.left + BUTTON_WIDTH;
	region_exit_button.top = 650;
	region_exit_button.bottom = region_exit_button.top + BUTTON_HEIGHT;

	StartGameButton button_start_game = StartGameButton(region_start_button,
		_T("pic/start_idle.png"), _T("pic/start_hanging.png"), _T("pic/start_clicked.png"));
	ExitGameButton button_exit_game = ExitGameButton(region_exit_button,
		_T("pic/exit_idle.png"), _T("pic/exit_hanging.png"), _T("pic/exit_clicked.png"));

	loadimage(&img_menu, _T("pic/menu_background.png"));
	mciSendString(_T("open mus/bgm.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("open mus/right.mp3 alias right"), NULL, 0, NULL);
	mciSendString(_T("open mus/wrong.mp3 alias wrong"), NULL, 0, NULL);
	mciSendString(_T("open mus/win.wav alias win"), NULL, 0, NULL);
	mciSendString(_T("open mus/loss.mp3 alias loss"), NULL, 0, NULL);
	mciSendString(_T("open mus/clicked.mp3 alias clicked"), NULL, 0, NULL);
	mciSendString(_T("open mus/hanging.mp3 alias hanging"), NULL, 0, NULL);

	BeginBatchDraw();

	while (running)
	{
		DWORD start_time = GetTickCount();

		// read process draw
		while (peekmessage(&msg))
		{
			if (is_game_started) NowScene->player.ProcessEvent(msg);
			else {
				button_start_game.ProcessEvent(msg);
				button_exit_game.ProcessEvent(msg);
			}
		}

		if (is_game_started) NowScene->player.Move(NowScene->airwall);

		cleardevice();

		if (is_game_started) {
			NowScene->Draw(1000 / 90);
			NowScene->checkSceneChange(NowScene);
			Scene::PrintNowTime();
		}
		else {
			putimage(0, 0, &img_menu);
			button_start_game.Draw();
			button_exit_game.Draw();
		}


		FlushBatchDraw();

		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;

		if (delta_time < 1000 / 90)
		{
			Sleep(1000 / 90 - delta_time);
		}

	}

	Delete_all_scene_vector();
	EndBatchDraw();

	return 0;
}