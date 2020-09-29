/*
* Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#include "itkObjectFactoryBase.h"
#include "itkFastMutexLock.h"
#include "itkMetaImageIOFactory.h"

namespace vns
{

template<typename TFactory>
class AtomicRegistry
{
public:
  AtomicRegistry()
    {
    m_Lock.Lock();
    if (m_Factory == ITK_NULLPTR)
      {
      typename TFactory::Pointer fac = TFactory::New();
      //itk::ObjectFactoryBase::UnRegisterFactory(factory);
      itk::ObjectFactoryBase::RegisterFactory(fac);
      m_Factory = fac;
      }
    m_Factory->Register();
    m_Lock.Unlock();
    }

  ~AtomicRegistry()
    {
    m_Lock.Lock();
    assert(m_Factory->GetReferenceCount() >= 2);
    m_Factory->UnRegister();
    if (m_Factory->GetReferenceCount() == 1)
      {
      itk::ObjectFactoryBase::UnRegisterFactory(m_Factory);
      m_Factory = ITK_NULLPTR;
      }
    m_Lock.Unlock();
    }

protected:

private:
  static itk::ObjectFactoryBase * m_Factory;

  itk::SimpleFastMutexLock m_Lock;
};

template<typename TFactory>
itk::ObjectFactoryBase * AtomicRegistry<TFactory>::m_Factory = ITK_NULLPTR;

/* Explicit instanciation */
template class AtomicRegistry<itk::MetaImageIOFactory>;

}
