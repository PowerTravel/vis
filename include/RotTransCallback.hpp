#ifndef ROT_TRANS_CALLBACK_HPP
#define ROT_TRANS_CALLBACK_HPP
#include "Transform.hpp"

class RotTransCallback : public UpdateCallback{

	public:
		RotTransCallback(transform_ptr trns, float angle = 0.1, vec3 axis = vec3(0,1,0));
		virtual ~RotTransCallback();

		void execute();	
	
	private:
		transform_ptr _trns;
		vec3 _axis;
		float _angle;
}; 



#endif // ROT_TRANS_CALLBACK_HPP
