//-----------------------------------------------------------------------------
// Copyright (c) 2013 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef _ASSET_BASE_H_
#include "assetBase.h"
#endif

#ifndef _ASSET_MANAGER_H_
#include "assetManager.h"
#endif

#ifndef _CONSOLETYPES_H_
#include "console/consoleTypes.h"
#endif

// Script bindings.
#include "assetBase_ScriptBinding.h"

// Debug Profiling.
#include "platform/profiler.h"

//-----------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(AssetBase);

//-----------------------------------------------------------------------------

StringTableEntry assetNameField = StringTable->insert("AssetName");
StringTableEntry assetDescriptionField = StringTable->insert("AssetDescription");
StringTableEntry assetCategoryField = StringTable->insert("AssetCategory");
StringTableEntry assetAutoUnloadField = StringTable->insert("AssetAutoUnload");
StringTableEntry assetInternalField = StringTable->insert("AssetInternal");
StringTableEntry assetPrivateField = StringTable->insert("AssetPrivate");

StringTableEntry assetSourceURLField = StringTable->insert("AssetSourceURL");
StringTableEntry assetAuthorNameField = StringTable->insert("AssetAuthorName");
StringTableEntry assetLicenseField = StringTable->insert("AssetLicense");
StringTableEntry assetAuthorURLField = StringTable->insert("AssetAuthorURL");

//-----------------------------------------------------------------------------
const String AssetBase::mErrCodeStrings[] =
{
   "Failed",
   "Ok",
   "NotLoaded",
   "BadFileReference",
   "InvalidFormat",
   "DependencyNotFound",
   "FileTooLarge",
   "UsingFallback",
   "UnKnown"
};

AssetBase::AssetBase() :
mpOwningAssetManager(NULL),
mAcquireReferenceCount(0),
mAssetInitialized(false)
{
   // Generate an asset definition.
   mpAssetDefinition = new AssetDefinition();

   mInternalName = StringTable->EmptyString();
   mClassName = StringTable->EmptyString();
   mSuperClassName = StringTable->EmptyString();
   mLoadedState = AssetErrCode::NotLoaded;
}

//-----------------------------------------------------------------------------

AssetBase::~AssetBase()
{
   // If the asset manager does not own the asset then we own the
   // asset definition so delete it.
   if (!getOwned())
      SAFE_DELETE(mpAssetDefinition);
}

//-----------------------------------------------------------------------------

void AssetBase::initPersistFields()
{
   // Call parent.
   Parent::initPersistFields();

   // Asset configuration.
   addProtectedField(assetNameField, TypeString, 0, &setAssetName, &getAssetName, &writeAssetName, "The name of the asset.  The is not a unique identification like an asset Id.");
   addProtectedField(assetDescriptionField, TypeString, 0, &setAssetDescription, &getAssetDescription, &writeAssetDescription, "The simple description of the asset contents.");
   addProtectedField(assetCategoryField, TypeString, 0, &setAssetCategory, &getAssetCategory, &writeAssetCategory, "An arbitrary category that can be used to categorized assets.");
   addProtectedField(assetAutoUnloadField, TypeBool, 0, &setAssetAutoUnload, &getAssetAutoUnload, &writeAssetAutoUnload, "Whether the asset is automatically unloaded when an asset is released and has no other acquisitions or not.");
   addProtectedField(assetInternalField, TypeBool, 0, &setAssetInternal, &getAssetInternal, &writeAssetInternal, "Whether the asset is used internally only or not.");
   addProtectedField(assetPrivateField, TypeBool, 0, &defaultProtectedNotSetFn, &getAssetPrivate, &defaultProtectedNotWriteFn, "Whether the asset is private or not.");

   // Asset licensing / source information
   addProtectedField(assetSourceURLField, TypeString, 0, &setAssetSourceURL, &getAssetSourceURL, &writeAssetSourceURL, "The URL the asset has come from.");
   addProtectedField(assetAuthorNameField, TypeString, 0, &setAssetAuthorName, &getAssetAuthorName, &writeAssetAuthorName, "The name of the asset.  The is not a unique identification like an asset Id.");
   addProtectedField(assetLicenseField, TypeString, 0, &setAssetLicense, &getAssetLicense, &writeAssetLicense, "The name of the asset.  The is not a unique identification like an asset Id.");
   addProtectedField(assetAuthorURLField, TypeString, 0, &setAssetAuthorURL, &getAssetAuthorURL, &writeAssetAuthorURL, "The name of the asset.  The is not a unique identification like an asset Id.");
}

//------------------------------------------------------------------------------

void AssetBase::copyTo(SimObject* object)
{
   // Call to parent.
   Parent::copyTo(object);

   // Cast to asset.
   AssetBase* pAsset = static_cast<AssetBase*>(object);

   // Sanity!
   AssertFatal(pAsset != NULL, "AssetBase::copyTo() - Object is not the correct type.");

   // Copy state.
   pAsset->setAssetName(getAssetName());
   pAsset->setAssetDescription(getAssetDescription());
   pAsset->setAssetCategory(getAssetCategory());
   pAsset->setAssetAutoUnload(getAssetAutoUnload());
   pAsset->setAssetInternal(getAssetInternal());
   pAsset->setAssetAuthorName(getAssetAuthorName());
   pAsset->setAssetLicense(getAssetLicense());
   pAsset->setAssetAuthorURL(getAssetAuthorURL());
   pAsset->setAssetSourceURL(getAssetSourceURL());
}

//-----------------------------------------------------------------------------

void AssetBase::setAssetDescription(const char* pAssetDescription)
{
   // Fetch asset description.
   StringTableEntry assetDescription = StringTable->insert(pAssetDescription);

   // Ignore no change.
   if (mpAssetDefinition->mAssetDescription == assetDescription)
      return;

   // Update.
   mpAssetDefinition->mAssetDescription = assetDescription;

   // Refresh the asset.
   refreshAsset();
}

//-----------------------------------------------------------------------------

void AssetBase::setAssetCategory(const char* pAssetCategory)
{
   // Fetch asset category.
   StringTableEntry assetCategory = StringTable->insert(pAssetCategory);

   // Ignore no change.
   if (mpAssetDefinition->mAssetCategory == assetCategory)
      return;

   // Update.
   mpAssetDefinition->mAssetCategory = assetCategory;

   // Refresh the asset.
   refreshAsset();
}

//-----------------------------------------------------------------------------

void AssetBase::setAssetAutoUnload(const bool autoUnload)
{
   // Ignore no change.
   if (mpAssetDefinition->mAssetAutoUnload == autoUnload)
      return;

   // Update.
   mpAssetDefinition->mAssetAutoUnload = autoUnload;

   // Refresh the asset.
   refreshAsset();
}

//-----------------------------------------------------------------------------

void AssetBase::setAssetInternal(const bool assetInternal)
{
   // Ignore no change,
   if (mpAssetDefinition->mAssetInternal == assetInternal)
      return;

   // Update.
   mpAssetDefinition->mAssetInternal = assetInternal;

   // Refresh the asset.
   refreshAsset();
}

//-----------------------------------------------------------------------------

StringTableEntry AssetBase::expandAssetFilePath(const char* pAssetFilePath) const
{
   // Debug Profiling.
   PROFILE_SCOPE(AssetBase_ExpandAssetFilePath);

   // Sanity!
   AssertFatal(pAssetFilePath != NULL, "Cannot expand a NULL asset path.");

   // Fetch asset file-path length.
   const U32 assetFilePathLength = dStrlen(pAssetFilePath);

   // Are there any characters in the path?
   if (assetFilePathLength == 0)
   {
      // No, so return empty.
      return StringTable->EmptyString();
   }

   // Fetch the asset base-path hint.
   StringTableEntry assetBasePathHint;
   if (getOwned() && !getAssetPrivate())
   {
      assetBasePathHint = mpOwningAssetManager->getAssetPath(getAssetId());
   }
   else
   {
      assetBasePathHint = NULL;
   }

   // Expand the path with the asset base-path hint.
   char assetFilePathBuffer[1024];
   Con::expandPath(assetFilePathBuffer, sizeof(assetFilePathBuffer), pAssetFilePath, assetBasePathHint);
   return StringTable->insert(assetFilePathBuffer);
}

//-----------------------------------------------------------------------------

StringTableEntry AssetBase::collapseAssetFilePath(const char* pAssetFilePath) const
{
   // Debug Profiling.
   PROFILE_SCOPE(AssetBase_CollapseAssetFilePath);

   // Sanity!
   AssertFatal(pAssetFilePath != NULL, "Cannot collapse a NULL asset path.");

   // Fetch asset file-path length.
   const U32 assetFilePathLength = dStrlen(pAssetFilePath);

   // Are there any characters in the path?
   if (assetFilePathLength == 0)
   {
      // No, so return empty.
      return StringTable->EmptyString();
   }

   char assetFilePathBuffer[1024];

   // Is the asset not owned or private?
   if (!getOwned() || getAssetPrivate())
   {
      // Yes, so we can only collapse the path using the platform layer.
      Con::collapsePath(assetFilePathBuffer, sizeof(assetFilePathBuffer), pAssetFilePath);
      return StringTable->insert(assetFilePathBuffer);
   }

   // Fetch asset base-path.
   StringTableEntry assetBasePath = mpOwningAssetManager->getAssetPath(getAssetId());

   // Is the asset file-path location within the asset base-path?
   if (Con::isBasePath(pAssetFilePath, assetBasePath))
   {
      // Yes, so fetch path relative to the asset base-path.
      StringTableEntry relativePath = Platform::makeRelativePathName(pAssetFilePath, assetBasePath);

      // Format the collapsed path.
      dSprintf(assetFilePathBuffer, sizeof(assetFilePathBuffer), "%s", relativePath);
   }
   else
   {
      // No, so we can collapse the path using the platform layer.
      Con::collapsePath(assetFilePathBuffer, sizeof(assetFilePathBuffer), pAssetFilePath);
   }

   return StringTable->insert(assetFilePathBuffer);
}

//-----------------------------------------------------------------------------

void AssetBase::refreshAsset(void)
{
   // Debug Profiling.
   PROFILE_SCOPE(AssetBase_RefreshAsset);

   // Finish if asset is not owned or is not initialized.
   if (mpOwningAssetManager == NULL || !mAssetInitialized)
      return;

   // Yes, so refresh the asset via the asset manager.
   mpOwningAssetManager->refreshAsset(getAssetId());
}

//-----------------------------------------------------------------------------

S32 AssetBase::getAssetDependencyFieldCount(const char* pFieldName)
{
   S32 matchedFieldCount = 0;
   SimFieldDictionary* fieldDictionary = getFieldDictionary();
   for (SimFieldDictionaryIterator itr(fieldDictionary); *itr; ++itr)
   {
      SimFieldDictionary::Entry* entry = *itr;

      if (String(entry->slotName).startsWith(pFieldName))
      {
         matchedFieldCount++;
      }
   }

   return matchedFieldCount;
}

//-----------------------------------------------------------------------------

StringTableEntry AssetBase::getAssetDependencyField(const char* pFieldName, S32 index)
{
   SimFieldDictionary* fieldDictionary = getFieldDictionary();
   for (SimFieldDictionaryIterator itr(fieldDictionary); *itr; ++itr)
   {
      SimFieldDictionary::Entry* entry = *itr;

      String slotName = String(entry->slotName);

      if (slotName.startsWith(pFieldName))
      {
         S32 trailingNum;
         String::GetTrailingNumber(slotName.c_str(), trailingNum);

         if (trailingNum == index)
         {
            return StringTable->insert(String(entry->value).replace(ASSET_ID_FIELD_PREFIX, "").c_str());
         }
      }
   }

   return StringTable->EmptyString();
}

//-----------------------------------------------------------------------------

void AssetBase::clearAssetDependencyFields(const char* pFieldName)
{
   SimFieldDictionary* fieldDictionary = getFieldDictionary();
   for (SimFieldDictionaryIterator itr(fieldDictionary); *itr; ++itr)
   {
      SimFieldDictionary::Entry* entry = *itr;

      if (String(entry->slotName).startsWith(pFieldName))
      {
         setDataField(entry->slotName, NULL, "");
      }
   }
}

//-----------------------------------------------------------------------------

void AssetBase::addAssetDependencyField(const char* pFieldName, const char* pAssetId)
{
   U32 existingFieldCount = getAssetDependencyFieldCount(pFieldName);

   //we have a match!
   char depSlotName[50];
   dSprintf(depSlotName, sizeof(depSlotName), "%s%d", pFieldName, existingFieldCount);

   char depValue[255];
   dSprintf(depValue, sizeof(depValue), "%s=%s", ASSET_ID_SIGNATURE, pAssetId);

   setDataField(StringTable->insert(depSlotName), NULL, StringTable->insert(depValue));
}

//-----------------------------------------------------------------------------
bool AssetBase::saveAsset()
{
   // Set the format mode.
   Taml taml;

   // Yes, so set it.
   taml.setFormatMode(Taml::getFormatModeEnum("xml"));

   // Turn-off auto-formatting.
   taml.setAutoFormat(false);

   // Read object.
   bool success = taml.write(this, AssetDatabase.getAssetFilePath(getAssetId()));

   if (!success)
      return false;

   return true;
}

//-----------------------------------------------------------------------------

void AssetBase::acquireAssetReference(void)
{
   // Acquired the acquired reference count.
   if (mpOwningAssetManager != NULL)
      mpOwningAssetManager->acquireAcquiredReferenceCount();

   mAcquireReferenceCount++;
}

//-----------------------------------------------------------------------------

bool AssetBase::releaseAssetReference(void)
{
   // Are there any acquisition references?
   if (mAcquireReferenceCount == 0)
   {
      // Return "unload" unless auto unload is off.
      return mpAssetDefinition->mAssetAutoUnload;
   }

   // Release the acquired reference count.
   if (mpOwningAssetManager != NULL)
      mpOwningAssetManager->releaseAcquiredReferenceCount();

   // Release reference.
   mAcquireReferenceCount--;

   // Are there any acquisition references?
   if (mAcquireReferenceCount == 0)
   {
      // No, so return "unload" unless auto unload is off.
      return mpAssetDefinition->mAssetAutoUnload;
   }

   // Return "don't unload".
   return false;
}

//-----------------------------------------------------------------------------

void AssetBase::setOwned(AssetManager* pAssetManager, AssetDefinition* pAssetDefinition)
{
   // Debug Profiling.
   PROFILE_SCOPE(AssetBase_setOwned);

   // Sanity!
   AssertFatal(pAssetManager != NULL, "Cannot set asset ownership with NULL asset manager.");
   AssertFatal(mpOwningAssetManager == NULL, "Cannot set asset ownership if it is already owned.");
   AssertFatal(pAssetDefinition != NULL, "Cannot set asset ownership with a NULL asset definition.");
   AssertFatal(mpAssetDefinition != NULL, "Asset ownership assigned but has a NULL asset definition.");
   AssertFatal(mpAssetDefinition->mAssetName == pAssetDefinition->mAssetName, "Asset ownership differs by asset name.");
   AssertFatal(mpAssetDefinition->mAssetDescription == pAssetDefinition->mAssetDescription, "Asset ownership differs by asset description.");
   AssertFatal(mpAssetDefinition->mAssetCategory == pAssetDefinition->mAssetCategory, "Asset ownership differs by asset category.");
   AssertFatal(mpAssetDefinition->mAssetAutoUnload == pAssetDefinition->mAssetAutoUnload, "Asset ownership differs by asset auto-unload flag.");
   AssertFatal(mpAssetDefinition->mAssetInternal == pAssetDefinition->mAssetInternal, "Asset ownership differs by asset internal flag.");

   AssertFatal(mpAssetDefinition->mAssetSourceURL == pAssetDefinition->mAssetSourceURL, "Asset ownership differs by source URL.");
   AssertFatal(mpAssetDefinition->mAssetAuthorURL == pAssetDefinition->mAssetAuthorURL, "Asset ownership differs by author URL.");
   AssertFatal(mpAssetDefinition->mAssetLicense == pAssetDefinition->mAssetLicense, "Asset ownership differs by license.");
   AssertFatal(mpAssetDefinition->mAssetAuthorName == pAssetDefinition->mAssetAuthorName, "Asset ownership differs by author name.");


   // Transfer asset definition ownership state.
   delete mpAssetDefinition;
   mpAssetDefinition = pAssetDefinition;

   // Flag as owned.
   // NOTE: This must be done prior to initializing the asset so any initialization can assume ownership.
   mpOwningAssetManager = pAssetManager;

   // Initialize the asset.
   initializeAsset();

   // Flag asset as initialized.
   mAssetInitialized = true;
}
