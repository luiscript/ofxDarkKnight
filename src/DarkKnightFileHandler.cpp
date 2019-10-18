/*
 Copyright (C) 2019 Luis Fernando Garcia [http://luiscript.com]

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "DarkKnightFileHandler.hpp"

DarkKnightFileHandler::DarkKnightFileHandler()
{
	modulesReference = nullptr;
	wiresReference = nullptr;
	openProjectAndLoad = false;
}

DarkKnightFileHandler::DarkKnightFileHandler(unordered_map<string, Module*>* modulesReferencePtr, vector<Wire>* wiresReferencePtr)
{
	modulesReference = modulesReferencePtr;
	wiresReference = wiresReferencePtr;
	openProjectAndLoad = false;
}

DarkKnightFileHandler::~DarkKnightFileHandler()
{
	modulesReference = nullptr;
	wiresReference = nullptr;
}


void DarkKnightFileHandler::saveFileDialog()
{
	ofFileDialogResult saveFileResult =
		ofSystemSaveDialog("project.batmapp", "Save project");

	if (saveFileResult.bSuccess) {
		saveProject(saveFileResult.getPath(), saveFileResult.getName());
	}
}

ofXml DarkKnightFileHandler::openFileDialog()
{
	ofFileDialogResult loadFileResult = ofSystemLoadDialog("Open batmapp project");
	if (loadFileResult.bSuccess)
	{
		modulesReference->clear();
		wiresReference->clear();
		return loadProject(loadFileResult.getPath(), loadFileResult.getName());
	}
}

void DarkKnightFileHandler::saveProject(string path, string name)
{
	
	auto project = xml.appendChild("project");

	// <resolution width = "" height = "" / >
	auto resolution = project.appendChild("resolution");
	resolution.setAttribute("width", 1920);
	resolution.setAttribute("height", 1080);

	// <gui translationX="0" translationY="0" zoom="1.0" />
	auto gui = project.appendChild("gui");
	gui.setAttribute("translationX", 0);
	gui.setAttribute("translationY", 0);
	gui.setAttribute("zoom", 1.0);

	// <modules>
	auto modules = project.appendChild("modules");
	for (auto moduleObject : *modulesReference)
	{
		// <module name="" id="" numChilds="" isChild="">
		auto module = modules.appendChild("module");
		module.setAttribute("name", moduleObject.first);
		module.setAttribute("id", moduleObject.second->getModuleId());
		module.setAttribute("isChild", moduleObject.second->moduleIsChild);
		auto modulePosition = module.appendChild("position");
		modulePosition.setAttribute("x", moduleObject.second->gui->getPosition().x);
		modulePosition.setAttribute("y", moduleObject.second->gui->getPosition().y);
		if (moduleObject.second->getModuleHasChild())
		{
			auto mediaPool = static_cast<MediaPool*>(moduleObject.second);
			int numChilds = mediaPool->collection.size();
			module.setAttribute("numChilds", numChilds);

			auto items = module.appendChild("items");
			for (auto collectionItem : mediaPool->collection)
			{
				auto item = items.appendChild("item");
				item.setAttribute("name", collectionItem.name);
				item.setAttribute("id", collectionItem.canvas->getModuleId());
				auto params = item.appendChild("params");
				for (auto moduleParam : collectionItem.canvas->gui->getItems())
				{
					auto param = params.appendChild("param");
					param.setAttribute("name", moduleParam->getName());
					auto paramType = moduleParam->getType();
					param.setAttribute("type", (int) paramType);
					if (paramType == ofxDatGuiType::SLIDER)
					{
						auto slider = static_cast<ofxDatGuiSlider*>(moduleParam);
						param.setAttribute("scale", slider->getScale());
					}
					else if (paramType == ofxDatGuiType::FOLDER)
					{
						auto folder = static_cast<ofxDatGuiFolder*>(moduleParam);
						for (auto folderItem : folder->children)
						{
							auto paramItem = param.appendChild("paramItem");
							paramItem.setAttribute("name", folderItem->getName());
							auto folderItemType = folderItem->getType();
							paramItem.setAttribute("type", (int) folderItemType);
							if (folderItemType == ofxDatGuiType::SLIDER)
							{
								auto folderSlider = static_cast<ofxDatGuiSlider*>(folderItem);
								paramItem.setAttribute("scale", folderSlider->getScale());
							}
						}
					}
					
				}
			}
		}
		else if(!moduleObject.second->moduleIsChild)
		{
			module.setAttribute("numChilds", 0);
			auto params = module.appendChild("params");
			for (auto moduleParam : moduleObject.second->gui->getItems())
			{
				auto param = params.appendChild("param");
				param.setAttribute("name", moduleParam->getName());
				auto moduleParamType = moduleParam->getType();
				param.setAttribute("type", (int) moduleParamType);
				if (moduleParamType == ofxDatGuiType::SLIDER) 
				{
					auto moduleSlider = static_cast<ofxDatGuiSlider*>(moduleParam);
					param.setAttribute("scale", moduleSlider->getScale());
				}
			}
		}
	}

	xml.save(path + ".xml");
}


ofXml DarkKnightFileHandler::loadProject(string path, string name)
{
	xml.load(path);
	return xml;
	/*
	xml.load(path + name);
	auto project = xml.getChild("project");
	if (!project)	
	{
		cout << "Incorrect file format" << endl;
		return;
	}
	auto resolution = project.getChild("resolution");
	ofVec2f resolutionVector = 
	{	
		(float)resolution.getAttribute("width"), 
		(float)resolution.getAttribute("height") 
	};

	auto gui = project.getChild("gui");
	ofVec3f guiVector =
	{
		(float)gui.getAttribute("translationX"),
		(float)gui.getAttribute("translationY"),
		(float)gui.getAttribute("zoom")
	};

	auto modules = project.getChild("modules");
	if (!modules) {
		cout << "This file does not have any modules" << endl;
		return;
	}
	for (auto moduleObject : modules.getChildren())
	{
		auto name = moduleObject.getAttribute("name");
		string moduleName = name.getValue();
		moduleName = moduleName.substr(0, moduleName.find("-"));
		int id = moduleObject.getAttribute("id").getIntValue();
	}
	*/

	//addModule("testing this shit");

	openProjectAndLoad = true;
}


/*template<typename T>
void DarkKnightFileHandler::addModule(T*& p)
{
	p->addModule("PREVIEW");
}*/


bool DarkKnightFileHandler::openProject()
{
	return openProjectAndLoad;

}

void DarkKnightFileHandler::setOpenProject(bool p)
{
	openProjectAndLoad = p;
}

void DarkKnightFileHandler::setModulesReference(unordered_map<string, Module*>* ref)
{
	modulesReference = ref;
}

void DarkKnightFileHandler::setWiresReference(vector<Wire>* ref)
{
	wiresReference = ref;
}