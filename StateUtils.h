#ifndef STATEUTILS_H
#define STATEUTILS_H

// Create a global state state_name associated with the entity class entity_name
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

// Create a state state_name associated with the entity class entity_name
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
