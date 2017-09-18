#ifndef STATEUTILS_H
#define STATEUTILS_H

#define ENTITY_GLOBAL_STATE(state_name, entity_name)  class state_name : public State<entity_name> \
	{																	\
	private:															\
		state_name(){}													\
		state_name(const state_name&);									\
		state_name& operator=(const state_name&);						\
																		\
	public:																\
		static state_name* Instance();									\
																		\
		virtual void Enter(entity_name* entity){}						\
																		\
		virtual void Execute(entity_name* entity);						\
																		\
		virtual void Exit(entity_name* entity){}						\
																		\
		virtual bool OnMessage(entity_name* entity, const Telegram& msg); \
	}

#define ENTITY_STATE(state_name, entity_name) class state_name : public State<entity_name> \
	{																	\
	private:															\
		state_name(){}													\
		state_name(const state_name&);									\
		state_name& operator=(const state_name&);						\
																		\
	public:																\
		static state_name* Instance();									\
																		\
		virtual void Enter(entity_name* entity);						\
																		\
		virtual void Execute(entity_name* entity);						\
																		\
		virtual void Exit(entity_name* entity);							\
																		\
		virtual bool OnMessage(entity_name* entity, const Telegram& msg); \
	}

#endif
