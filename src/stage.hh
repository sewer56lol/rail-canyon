#pragma once
#include "common.hh"
#include "render/BSPModel.hh"
#include "io/ONEArchive.hh"
#include <list>
#include <bigg.hpp>
#include "render/TexDictionary.hh"
#include "render/TXCAnimation.hh"
#include "render/DFFModel.hh"

class VisibilityManager {
	struct VisibilityBlock {
		i32 chunk;
		i32 low_x;
		i32 low_y;
		i32 low_z;
		i32 high_x;
		i32 high_y;
		i32 high_z;

		inline bool contains(glm::vec3 pos) {
			i32 x = (i32) pos.x;
			i32 y = (i32) pos.y;
			i32 z = (i32) pos.z;

			return x >= low_x && y >= low_y && z >= low_z &&
				   x < high_x && y < high_y && z < high_z;
		}
	};
	std::vector<VisibilityBlock> blocks;
	bool fileExists = false;
	bool showChunkBorders = false;
	bool forceShowAll = false;
	bool usePadding = true;
public:
	bool isVisible(int chunkId, glm::vec3 camPos);
	void read(FSPath& blkFile);

	void drawUI(glm::vec3 camPos);

	void drawDebug(glm::vec3 camPos);
};

class DFFCache {
private:
	std::map<std::string, DFFModel*> cache;
public:
	void addFromArchive(FSPath& onePath, TexDictionary* txd);
	DFFModel* getDFF(const char* name);
};

class ObjectLayout {
private:
	struct ObjectInstance {
		float pos_x;
		float pos_y;
		float pos_z;
		u32 rot_x;
		u32 rot_y;
		u32 rot_z;
		u16 type;
		u8 linkID;
		u8 radius;
		u8 misc[32];
	};

	std::vector<ObjectInstance> objects;
public:
	void read(FSPath& binFile);

	void draw(glm::vec3 camPos, DFFCache* cache);
	void drawUI(glm::vec3 camPos);
};

class Stage {
	std::list<BSPModel> models;
	VisibilityManager visibilityManager;
	ObjectLayout* layout = nullptr;
	DFFCache* cache = nullptr;
public:
	~Stage();
	void fromArchive(ONEArchive* x, TexDictionary* txd);
	void readVisibility(FSPath& blkFile);
	void readLayout(FSPath& binFile);
	void draw(glm::vec3 camPos, TXCAnimation* txc);
	void drawUI(glm::vec3 camPos);
	void drawVisibilityUI(glm::vec3 camPos);
	void drawLayoutUI(glm::vec3 camPos);
	void drawDebug(glm::vec3 camPos);

	void readCache(FSPath& oneFile, TexDictionary* txd);
};
