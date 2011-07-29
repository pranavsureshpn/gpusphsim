#ifndef __SnowSim_h_
#define __SnowSim_h_

#include <Ogre.h>
#include "SimulationSystem.h"

namespace SnowSim
{
	struct GeneralSettings
	{
		int cudadevice;
		Ogre::LoggingLevel logLevel;
		bool showOgreConfigDialog;
		bool showOgreGui;
		Ogre::String fluidShader;
	};

	struct SceneSettings
	{
		Ogre::String skyBoxMaterial;
		bool cameraRelativeToFluid;
		Ogre::Vector3 cameraPosition;
		Ogre::Quaternion cameraOrientation;

		Ogre::Vector3 fluidPosition;
		Ogre::Vector3 terrainPosition;

		Ogre::ColourValue backgroundColor;
		Ogre::ColourValue fluidGridColor;

		int fluidScene;
	};

	struct FluidSettings
	{
		bool simpleSPH;
		bool enabled;
		bool enableKernelTiming;
		bool showFluidGrid;
		bool gridWallCollisions;
		bool terrainCollisions;
	};

	
	typedef std::vector<Ogre::String> TextureLayerFileList;
	typedef std::vector<Ogre::Real> TextureLayerSizeList;

	struct TerrainSettings
	{
		bool enabled;

		int size;

		// horizontal scale
		Ogre::Real worldSize;
		// vertical scale
		Ogre::Real worldScale;

		Ogre::String normalsDataFile;
		Ogre::String heightDataFile;

		// blend files
		TextureLayerFileList textureBlendFileList;

		// texture layer files
		TextureLayerFileList textureLayerDiffSpecFileList;
		TextureLayerFileList textureLayerNormalHeightFileList;
		TextureLayerSizeList textureLayerSize;
		bool flat;
		bool showDebugNormals;
	};

	struct MarchingCubeSettings
	{
		Ogre::Real BaseRadius;
		Ogre::Real SpaceResolution;
		std::size_t ThreadsPerBlock;
		std::size_t ParticleEffectCubesScope;
		Ogre::Real SamplingThreshold;
	};

	class Config
	{
	public:
		static Config* getSingletonPtr();
		~Config();

		GeneralSettings generalSettings;
		FluidSettings fluidSettings;
		TerrainSettings terrainSettings;
		SceneSettings sceneSettings;
		MarchingCubeSettings marchingcubeSettings;

		Ogre::ConfigFile* getCfg() { return mCfg; }

	private:
		Ogre::ConfigFile *mCfg;
		
		void loadConfig();
		void loadSceneConfig();
		void loadFluidConfig();
		void loadTerrainConfig();
		void loadMarchingCubeConfig();
		
		Config(const std::string& configFileName = "SnowSim.cfg");
		static Config *m_instance;
	};

}

#endif

