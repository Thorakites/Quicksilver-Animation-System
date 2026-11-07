	return result;
}


void CBAS_Actors::SetAttackSpeed(void* p){
//_DOUT("SetAttackSpeed: %p at %i...",p,GetTickCount());
	bool aadExist = AnimDataMap.count((uintptr_t)p) > 0;		//find out if this animdata pointer has already an entry in the map
	bool touched = false;	//has this pointer actually been processed fully or is it still not time
	
#if _DEBUG
	if(aadExist){
		//_MESSAGE("%p timings: (%i - %i = %i)",p,GetTickCount(),AnimDataMap.at((uintptr_t)p),(GetTickCount() - AnimDataMap.at((uintptr_t)p)));
	}
#endif

	if( aadExist && !TIME_PASSED(AnimDataMap.at((uintptr_t)p), TOUCH_INTERVAL) ){
//_DOUT("Not checking %p because too soon (%i - %i = %i)",p,GetTickCount(),AnimDataMap.at((uintptr_t)p),(GetTickCount() - AnimDataMap.at((uintptr_t)p)));
		return;			//skip any further processing on this pointer
	}

	touched = true;
//_DOUT("Received pointer %p, attempting to get animdata...",p);
	if ( Actor* a = ResolveActorFromAnimData(p) ){
		//if it's a creature, and we're ignoring creatures... ignore
		if( !GETINI(CBAS_Creatures) ) {
			_DOUT("Not using creatures!");
			if(TESCreature* creat = GetCreature(a)) {
				_DOUT("%s (%X) is a creature! Exiting...",GetFullName(a),a->refID);
				return;
			}
		}
//_DOUT("	GOT ACTOR FROM %p",p);
		//Change the time period of the animation to the desired attack speed
		_DOUT("CURRENT PERIOD IS: %f",(reinterpret_cast<CBAS_ActorAnimData*>(p))->t_period);
		float currentPeriod = (reinterpret_cast<CBAS_ActorAnimData*>(p))->t_period;
float cbasPeriod = AttackSpeedFromActor(a);

if (g_thePlayer && a == *g_thePlayer)
{
    _MESSAGE("[Quicksilver Test] Player period before: %f | CBAS value: %f",
        currentPeriod, cbasPeriod);
}

(reinterpret_cast<CBAS_ActorAnimData*>(p))->t_period = cbasPeriod;
	} else {
		//if we can't get actor
_DOUT("	Didn't get actor from %p",p);
	}

	if( !aadExist || touched ) {
		AnimDataMap[(uintptr_t)p] = GetTickCount();		//update map
_DOUT("Updating map at %x to be %i because either %p didn't exist: %i, or touched: %i",(uintptr_t)p,GetTickCount(),p,!aadExist,touched);
	}
}


Actor* CBAS_Actors::ResolveActorFromAnimData(void* p){
	if( CBAS_ActorAnimData* c_aad = reinterpret_cast<CBAS_ActorAnimData*>(p) ){
		if( NiNode* niNode = c_aad->niNode04 ){
			if( niNode->m_extraDataList && niNode->m_extraDataListLen > 2 ){
				NiExtraData* niExD = niNode->m_extraDataList[2];
				if( std::string(niExD->GetType()->name) == "TESObjectExtraData" ){
					if( Actor* ActorFromAnimData = reinterpret_cast<Actor*>(((TESObjectExtraData*)niExD)->refr) ){
						return ActorFromAnimData;
					PRSNK_ERR("		Failed to cast actor at last hurdle! %s",niExD->GetType()->name);
					}
				PRSNK_ERR("		NiExtraData does not match TESObjectExtraData! %s",niExD->GetType()->name);
				}
			PRSNK_ERR("		NiNode's extradatalist not found or too short from node at %p",niNode);
			}
			PRSNK_ERR("		NiNode not found from animdata at %p",c_aad);
		}
	PRSNK_ERR("		Animdata not found at %p",p);
	}

	return NULL;
}


#if 0

Actor* CBAS_Actors::ResolveActorFromAnimData(void* p){
	if( CBAS_ActorAnimData* c_aad = reinterpret_cast<CBAS_ActorAnimData*>(p) ){
		if( NiNode* niNode = c_aad->niNode04 ){
			if( niNode->m_extraDataList && niNode->m_extraDataListLen > 2 ){
				NiExtraData* niExD = niNode->m_extraDataList[2];
				if( std::string(niExD->GetType()->name) == "TESObjectExtraData" ){
					if( Actor* ActorFromAnimData = reinterpret_cast<Actor*>(((TESObjectExtraData*)niExD)->refr) ){
#ifdef _DEBUG
						_MESSAGE("	Got actor %s at last hurdle from: %s",GetFullName(ActorFromAnimData),ActorFromAnimData->refID,niNode->m_extraDataList[2]->GetType()->name);
#endif
						return ActorFromAnimData;
#ifdef _DEBUG
					} else {
						_MESSAGE("		Failed to cast actor at last hurdle! %s",niExD->GetType()->name);
#endif
					}
#ifdef _DEBUG
				} else {
					_MESSAGE("		NiExtraData does not match TESObjectExtraData! %s",niExD->GetType()->name);
#endif
				}
#ifdef _DEBUG
			} else {
				_MESSAGE("		NiNode's extradatalist not found or too short from node at %p",niNode);
#endif
			}
#ifdef _DEBUG
		} else {
			_MESSAGE("		NiNode not found from animdata at %p",c_aad);
#endif
		}
#ifdef _DEBUG
	} else {
			_MESSAGE("		Animdata not found at %p",p);
#endif
	}

	return NULL;
}

#endif
