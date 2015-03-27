#ifndef KEYSTATE_HPP
#define KEYSTATE_HPP


void glut_key_callback(unsigned char c, int x, int y);
void glut_special_key_callback(int c, int x, int y);
void resize_callback(int width, int height);

class KeyState{
	
	public:

		enum Key_State {
			KEY_NULL = 0,
			KEY_A = 1,
			KEY_B = 2,
			KEY_C = 4,
			KEY_D = 8,
			KEY_E = 16,
			KEY_F = 32,  
			KEY_G = 64,
			KEY_H = 128,
			KEY_I = 256,
			KEY_J = 512,
			KEY_K = 1024,
			KEY_L = 2048,
			KEY_M = 4096,
			KEY_N = 8192,
			KEY_O = 16384,
			KEY_P = 32768,
			KEY_Q = 65536,
			KEY_R = 131072,
			KEY_S = 262144,
			KEY_T = 524288,
			KEY_U = 1048576,
			KEY_V = 2097152,
			KEY_W = 4194304,
			KEY_X = 8388608,
			KEY_Y = 16777216,
			KEY_Z = 33554432
		};
		enum Special_Key_State {
			SPECIAL_KEY_NULL = 0,
			SPECIAL_KEY_UP = 1,
			SPECIAL_KEY_DOWN = 2,
			SPECIAL_KEY_LEFT = 4,
			SPECIAL_KEY_RIGHT = 8

		};

		static KeyState& getInstance(); 

		bool hasWindowChanged();
		void windowChanged();
		void setGlutCallback();
		Key_State get();
		Special_Key_State get_special();
		void set(int st);
		void special_set(int st);

	private:
		static Key_State _key;
		static Special_Key_State _special_key;
		static bool mWindowChanged;
		
		KeyState();
		virtual ~KeyState();
	
		KeyState(KeyState const&) = delete;
		void operator=(KeyState const&) = delete;
};

#ifndef KEY_STATE_BIT
#define KEY_STATE_BIT
typedef KeyState::Key_State key_state_bit;
typedef KeyState::Special_Key_State special_key_state_bit;
#endif // KEY_STATE_BIT


#endif // KEYSTATE
