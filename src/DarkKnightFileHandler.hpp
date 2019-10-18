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

#ifndef DarkKnightFileHandler_hpp
#define DarkKnightFileHandler_hpp

#include "ofMain.h"
#include "module.hpp"
#include "mediaPool.hpp"
#include "wire.hpp"
#include "wireConnection.hpp"
#include "unordered_map"


class DarkKnightFileHandler
{
private:
	ofXml xml;
	unordered_map<string, Module*>* modulesReference;
	vector<Wire>* wiresReference;
	bool openProjectAndLoad;

public:
	DarkKnightFileHandler();
	DarkKnightFileHandler(unordered_map<string, Module*>*, vector<Wire>*);
	~DarkKnightFileHandler();
	void saveFileDialog();
	ofXml openFileDialog();
	void saveProject(string, string);
	ofXml loadProject(string, string);

	bool openProject();
	void setOpenProject(bool);
	void setModulesReference(unordered_map<string, Module*>*);
	void setWiresReference(vector<Wire>*);
};


#endif