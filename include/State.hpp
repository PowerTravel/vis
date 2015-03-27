#ifndef STATE_HPP
#define STATE_HPP

class State{

	public:
		State();
		virtual ~State();

		void merge(State* s);

	private:
};

#endif // STATE_HPP
