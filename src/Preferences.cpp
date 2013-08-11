/*
 Copyright (C) 2010-2013 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Preferences.h"

namespace TrenchBroom {
    namespace Preferences {
#ifdef __APPLE__
        Preference<String> QuakePath("QuakePath", "/Applications/Quake");
        Preference<String> Quake2Path("Quake2Path", "/Applications/Quake2");
#else
        Preference<String> QuakePath("QuakePath", "");
        Preference<String> Quake2Path("Quake2Path", "");
#endif

        Preference<Color> BackgroundColor("Renderer/Colors/Background", Color(0.0f, 0.0f, 0.0f, 1.0f));
        Preference<Color> XAxisColor("Renderer/Colors/Background", Color(1.0f, 0.0f, 0.0f, 1.0f));
        Preference<Color> YAxisColor("Renderer/Colors/Background", Color(0.0f, 1.0f, 0.0f, 1.0f));
        Preference<Color> ZAxisColor("Renderer/Colors/Background", Color(0.0f, 0.0f, 1.0f, 1.0f));
        Preference<Color> FaceColor("Renderer/Colors/Faces", Color(0.2f,  0.2f,  0.2f,  1.0f));
        Preference<Color> SelectedFaceColor("Renderer/Colors/Selected faces", Color(0.6f,  0.35f, 0.35f, 1.0f));
        Preference<Color> EdgeColor("Renderer/Colors/Edges", Color(0.7f,  0.7f,  0.7f,  1.0f));
        Preference<Color> SelectedEdgeColor("Renderer/Colors/Selected edges", Color(1.0f,  0.0f,  0.0f,  1.0f));
        Preference<Color> OccludedSelectedEdgeColor("Renderer/Colors/Occluded selected edges", Color(1.0f,  0.0f,  0.0f,  0.5f));
        Preference<Color> UndefinedEntityColor("Renderer/Colors/Undefined entity", Color(0.5f,  0.5f,  0.5f,  1.0f));

        Preference<Color> InfoOverlayTextColor("Renderer/Colors/Info overlay text", Color(1.0f, 1.0f, 1.0f, 1.0f));
        Preference<Color> InfoOverlayBackgroundColor("Renderer/Colors/Info overlay background", Color(0.0f, 0.0f, 0.0f, 0.6f));
        Preference<Color> SelectedInfoOverlayTextColor("Renderer/Colors/Selected info overlay text", Color(1.0f, 1.0f, 1.0f, 1.0f));
        Preference<Color> SelectedInfoOverlayBackgroundColor("Renderer/Colors/Selected info overlay background", Color(1.0f, 0.0f, 0.0f, 0.6f));

        Preference<float> Brightness("Renderer/Brightness", 1.0f);
        Preference<float> GridAlpha("Renderer/Grid/Alpha", 0.5f);
        Preference<bool> GridCheckerboard("Renderer/Grid/Checkerboard", false);
        Preference<bool> ShadeFaces("Renderer/ShadeFaces", true);
        Preference<bool> UseFog("Renderer/UseFog", false);
        
#ifdef __APPLE__
        Preference<String> RendererFontName("Renderer/Font name", "LucidaGrande");
#else
        Preference<String> RendererFontName("Renderer/Font name", "Arial");
#endif
        Preference<int> RendererFontSize("Renderer/Font size", 13);

        Preference<float> CameraLookSpeed("Controls/Camera/Look speed", 0.5f);
        Preference<bool>  CameraLookInvertH("Controls/Camera/Invert horizontal look", false);
        Preference<bool>  CameraLookInvertV("Controls/Camera/Invert vertical look", false);
        Preference<float> CameraPanSpeed("Controls/Camera/Pan speed", 0.5f);
        Preference<bool>  CameraPanInvertH("Controls/Camera/Invert horizontal pan", false);
        Preference<bool>  CameraPanInvertV("Controls/Camera/Invert vertical pan", false);
        Preference<float> CameraMoveSpeed("Controls/Camera/Move speed", 0.3f);
        Preference<bool> CameraEnableAltMove("Controls/Camera/Use alt to move", false);
        Preference<bool> CameraMoveInCursorDir("Controls/Camera/Move camera in cursor dir", false);
    }
}
