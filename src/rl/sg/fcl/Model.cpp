//
// Copyright (c) 2009, Markus Rickert
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "Body.h"
#include "Model.h"
#include "Scene.h"

namespace rl
{
	namespace sg
	{
		namespace fcl
		{
			Model::Model(Scene* scene) :
				::rl::sg::Model(scene),
				manager()
			{
				this->manager.setup();
				this->getScene()->add(this);
			}
			
			Model::~Model()
			{
				while (this->bodies.size() > 0)
				{
					delete this->bodies[0];
				}
				
				this->getScene()->remove(this);
			}
			
			void
			Model::add(Body* body)
			{
				this->bodies.push_back(body);
				::std::vector<CollisionObject*> objects;
				body->manager.getObjects(objects);
				
				if (objects.size() > 0)
				{
					this->manager.registerObjects(objects);
				}
			}
			
			void
			Model::addCollisionObject(CollisionObject* collisionObject, Body* body)
			{
				this->manager.registerObject(collisionObject);
				dynamic_cast<Scene*>(this->getScene())->addCollisionObject(collisionObject, body);
			}
			
			::rl::sg::Body*
			Model::create()
			{
				return new Body(this);
			}
			
			void
			Model::remove(Body* body)
			{
				Iterator found = ::std::find(this->bodies.begin(), this->bodies.end(), body);
			
				if (found != this->bodies.end())
				{
					this->bodies.erase(found);
					::std::vector<CollisionObject*> objects;
					body->manager.getObjects(objects);
					
					for (::std::size_t i = 0; i < objects.size(); ++i)
					{
						this->manager.unregisterObject(objects[i]);
					}
				}
			}
			
			void
			Model::removeCollisionObject(CollisionObject* collisionObject)
			{
				this->manager.unregisterObject(collisionObject);
				dynamic_cast<Scene*>(this->getScene())->removeCollisionObject(collisionObject);
			}
		}
	}
}
