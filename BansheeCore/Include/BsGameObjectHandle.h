//__________________________ Banshee Project - A modern game development toolkit _________________________________//
//_____________________________________ www.banshee-project.com __________________________________________________//
//________________________ Copyright (c) 2014 Marko Pintera. All rights reserved. ________________________________//
#pragma once

namespace BansheeEngine
{
	class GameObjectManager;

	/**
	 * @brief	Internal data shared between GameObject handles.
	 */
	struct BS_CORE_EXPORT GameObjectHandleData
	{
		GameObjectHandleData()
			:mPtr(nullptr), mInstanceId(0)
		{ }

		GameObjectHandleData(const std::shared_ptr<GameObjectInstanceData>& ptr)
		{
			mPtr = ptr;
			if(ptr != nullptr)
				mInstanceId = ptr->object->getInstanceId();
			else
				mInstanceId = 0;
		}

		std::shared_ptr<GameObjectInstanceData> mPtr;
		UINT64 mInstanceId;
	};

	/**
	 * @brief	A handle that can point to various types of game objects.
	 * 			It primarily keeps track if the object is still alive, so anything
	 * 			still referencing it doesn't accidentally use it.
	 * 			
	 * @note	This class exists because references between game objects should be quite loose.
	 * 			For example one game object should be able to reference another one without the other
	 * 			one knowing. But if that is the case I also need to handle the case when the other
	 * 			object we're referencing has been deleted, and that is the main purpose of this class.
	 * 			
	 */
	class BS_CORE_EXPORT GameObjectHandleBase : public IReflectable
	{
	public:
		GameObjectHandleBase();

		/**
		 * @brief	Returns true if the object the handle is pointing to has been destroyed.
		 */
		bool isDestroyed() const { return mData->mPtr == nullptr || mData->mPtr->object == nullptr; }

		/**
		 * @brief	Returns the instance ID of the object the handle is referencing.
		 */
		UINT64 getInstanceId() const { return mData->mInstanceId; }

		/**
		 * @brief	Returns pointer to the referenced GameObject.
		 *
		 * @note	Throws exception if the GameObject was destroyed.
		 */
		GameObject* get() const 
		{ 
			throwIfDestroyed();

			return mData->mPtr->object.get(); 
		}

		/**
		 * @brief	Returns a smart pointer to the referenced GameObject.
		 *
		 * @note	Throws exception if the GameObject was destroyed.
		 */
		std::shared_ptr<GameObject> getInternalPtr() const
		{
			throwIfDestroyed();

			return mData->mPtr->object;
		}

		/**
		 * @brief	Returns pointer to the referenced GameObject.
		 *
		 * @note	Throws exception if the GameObject was destroyed.
		 */
		GameObject* operator->() const { return get(); }

		/**
		 * @brief	Returns reference to the referenced GameObject.
		 *
		 * @note	Throws exception if the GameObject was destroyed.
		 */
		GameObject& operator*() const { return *get(); }

		/**
		 * @brief	Returns internal handle data.
		 *
		 * @note	Internal method.
		 */
		std::shared_ptr<GameObjectHandleData> _getHandleData() const { return mData; }

		/**
		 * @brief	Resolves a handle to a proper GameObject in case it was created uninitialized.
		 *
		 * @note	Internal method.
		 */
		void _resolve(const GameObjectHandleBase& object);

	protected:
		friend class SceneObject;
		friend class SceneObjectRTTI;
		friend class GameObjectManager;

		GameObjectHandleBase(const std::shared_ptr<GameObject> ptr);
		GameObjectHandleBase(const std::shared_ptr<GameObjectHandleData>& data);
		GameObjectHandleBase(std::nullptr_t ptr);

		/**
		 * @brief	Throws an exception if the referenced GameObject has been destroyed.
		 */
		inline void throwIfDestroyed() const;
		
		/**
		 * @brief	Invalidates the handle signifiying the referenced object was destroyed.
		 */
		void destroy()
		{
			// We need to clear mData->mPtr before we clear mData->mPtr->object,
			// as this handle could be stored within the "object" and destroyed when
			// we set it to null 
			std::shared_ptr<GameObjectInstanceData> instanceData = mData->mPtr;
			mData->mPtr = nullptr;

			if(instanceData != nullptr)
				instanceData->object = nullptr;
		}

		std::shared_ptr<GameObjectHandleData> mData;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class GameObjectHandleRTTI;
		static RTTITypeBase* getRTTIStatic();
		virtual RTTITypeBase* getRTTI() const;
	};

	/**
	 * @copydoc	GameObjectHandleBase
	 *
	 * @note	It is important this class contains no data since we often 
	 *			value cast it to its base.
	 */
	template <typename T>
	class GameObjectHandle : public GameObjectHandleBase
	{
	public:
		/**
		 * @brief	Constructs a new empty handle.
		 */
		GameObjectHandle()
			:GameObjectHandleBase()
		{	
			mData = bs_shared_ptr<GameObjectHandleData, PoolAlloc>();
		}

		/**
		 * @brief	Copy constructor from another handle of the same type.
		 */
		template <typename T1>
		GameObjectHandle(const GameObjectHandle<T1>& ptr)
			:GameObjectHandleBase()
		{ 	
			mData = ptr._getHandleData();
		}

		/**
		 * @brief	Copy constructor from another handle of the base type.
		 */
		GameObjectHandle(const GameObjectHandleBase& ptr)
			:GameObjectHandleBase()
		{ 	
			mData = ptr._getHandleData();
		}

		/**
		 * @brief	Invalidates the handle.
		 */
		inline GameObjectHandle<T>& operator=(std::nullptr_t ptr)
		{ 	
			mData = bs_shared_ptr<GameObjectHandleData, PoolAlloc>();

			return *this;
		}

		/**
		 * @brief	Casts a specific handle to the base handle.
		 */
		inline operator GameObjectHandleBase()
		{
			GameObjectHandleBase base(mData);

			return base;
		}

		/**
		 * @brief	Returns a pointer to the referenced GameObject.
		 *
		 * @note	Throws exception if the GameObject was destroyed.
		 */
		T* get() const 
		{ 
			throwIfDestroyed();

			return reinterpret_cast<T*>(mData->mPtr->object.get()); 
		}

		/**
		 * @brief	Returns a smart pointer to the referenced GameObject.
		 *
		 * @note	Throws exception if the GameObject was destroyed.
		 */
		std::shared_ptr<T> getInternalPtr() const
		{
			throwIfDestroyed();

			return std::static_pointer_cast<T>(mData->mPtr->object);
		}

		/**
		 * @brief	Returns pointer to the referenced GameObject.
		 *
		 * @note	Throws exception if the GameObject was destroyed.
		 */
		T* operator->() const { return get(); }

		/**
		 * @brief	Returns reference to the referenced GameObject.
		 *
		 * @note	Throws exception if the GameObject was destroyed.
		 */
		T& operator*() const { return *get(); }

		template<class _Ty>
		struct Bool_struct
		{
			int _Member;
		};

		/**
		 * @brief	Allows direct conversion of handle to bool.
		 *
		 * @note	This is needed because we can't directly convert to bool
		 *			since then we can assign pointer to bool and that's weird.
		 */
		operator int Bool_struct<T>::*() const
		{
			return (((mData->mPtr != nullptr) && (mData->mPtr->object != nullptr)) ? &Bool_struct<T>::_Member : 0);
		}

	private:
		friend class SceneObject;
		friend class SceneObjectRTTI;
		friend class GameObjectManager;

		/**
		 * @brief	Creates a handle from a smart pointer.
		 */
		explicit GameObjectHandle(const std::shared_ptr<T> ptr)
			:GameObjectHandleBase(ptr)
		{ }
	};

	/**
	 * @brief	Casts one handle type to another.
	 */
	template<class _Ty1, class _Ty2>
		GameObjectHandle<_Ty1> static_object_cast(const GameObjectHandle<_Ty2>& other)
	{	
		return GameObjectHandle<_Ty1>(other);
	}

	/**
	 * @brief	Compares if two handles point to the same GameObject.
	 */
	template<class _Ty1, class _Ty2>
	bool operator==(const GameObjectHandle<_Ty1>& _Left, const GameObjectHandle<_Ty2>& _Right)
	{	
		return (_Left == nullptr && _Right == nullptr) || (_Left != nullptr && _Right != nullptr && _Left.get() == _Right.get());
	}

	/**
	 * @brief	Compares if two handles point to different GameObjects.
	 */
	template<class _Ty1, class _Ty2>
	bool operator!=(const GameObjectHandle<_Ty1>& _Left, const GameObjectHandle<_Ty2>& _Right)
	{	
		return (!(_Left == _Right));
	}
}