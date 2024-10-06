/*
 Copyright (C) 2010 Kristian Duske

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
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include "SelectionCommand.h"

#include "Macros.h"
#include "View/MapDocumentCommandFacade.h"
#include "mdl/BrushFaceHandle.h"
#include "mdl/BrushFaceReference.h"

#include "kdl/result.h"
#include "kdl/string_format.h"

#include <sstream>
#include <string>

namespace tb::View
{
enum class SelectionCommand::Action
{
  SelectNodes,
  SelectFaces,
  SelectAllNodes,
  SelectAllFaces,
  ConvertToFaces,
  DeselectNodes,
  DeselectFaces,
  DeselectAll
};

std::unique_ptr<SelectionCommand> SelectionCommand::select(std::vector<mdl::Node*> nodes)
{
  return std::make_unique<SelectionCommand>(
    Action::SelectNodes, std::move(nodes), std::vector<mdl::BrushFaceHandle>{});
}

std::unique_ptr<SelectionCommand> SelectionCommand::select(
  std::vector<mdl::BrushFaceHandle> faces)
{
  return std::make_unique<SelectionCommand>(
    Action::SelectFaces, std::vector<mdl::Node*>{}, std::move(faces));
}

std::unique_ptr<SelectionCommand> SelectionCommand::convertToFaces()
{
  return std::make_unique<SelectionCommand>(
    Action::ConvertToFaces,
    std::vector<mdl::Node*>{},
    std::vector<mdl::BrushFaceHandle>{});
}

std::unique_ptr<SelectionCommand> SelectionCommand::selectAllNodes()
{
  return std::make_unique<SelectionCommand>(
    Action::SelectAllNodes,
    std::vector<mdl::Node*>{},
    std::vector<mdl::BrushFaceHandle>{});
}

std::unique_ptr<SelectionCommand> SelectionCommand::selectAllFaces()
{
  return std::make_unique<SelectionCommand>(
    Action::SelectAllFaces,
    std::vector<mdl::Node*>{},
    std::vector<mdl::BrushFaceHandle>{});
}

std::unique_ptr<SelectionCommand> SelectionCommand::deselect(
  std::vector<mdl::Node*> nodes)
{
  return std::make_unique<SelectionCommand>(
    Action::DeselectNodes, std::move(nodes), std::vector<mdl::BrushFaceHandle>{});
}

std::unique_ptr<SelectionCommand> SelectionCommand::deselect(
  std::vector<mdl::BrushFaceHandle> faces)
{
  return std::make_unique<SelectionCommand>(
    Action::DeselectFaces, std::vector<mdl::Node*>{}, std::move(faces));
}

std::unique_ptr<SelectionCommand> SelectionCommand::deselectAll()
{
  return std::make_unique<SelectionCommand>(
    Action::DeselectAll, std::vector<mdl::Node*>{}, std::vector<mdl::BrushFaceHandle>{});
}

SelectionCommand::SelectionCommand(
  const Action action,
  std::vector<mdl::Node*> nodes,
  std::vector<mdl::BrushFaceHandle> faces)
  : UndoableCommand{makeName(action, nodes.size(), faces.size()), false}
  , m_action{action}
  , m_nodes{std::move(nodes)}
  , m_faceRefs{mdl::createRefs(faces)}
{
}

SelectionCommand::~SelectionCommand() = default;

std::string SelectionCommand::makeName(
  const Action action, const size_t nodeCount, const size_t faceCount)
{
  std::stringstream result;
  switch (action)
  {
  case Action::SelectNodes:
    result << "Select " << nodeCount << " "
           << kdl::str_plural(nodeCount, "Object", "Objects");
    break;
  case Action::SelectFaces:
    result << "Select " << faceCount << " "
           << kdl::str_plural(faceCount, "Brush Face", "Brush Faces");
    break;
  case Action::SelectAllNodes:
    result << "Select All Objects";
    break;
  case Action::SelectAllFaces:
    result << "Select All Brush Faces";
    break;
  case Action::ConvertToFaces:
    result << "Convert to Brush Face Selection";
    break;
  case Action::DeselectNodes:
    result << "Deselect " << nodeCount << " "
           << kdl::str_plural(nodeCount, "Object", "Objects");
    break;
  case Action::DeselectFaces:
    result << "Deselect " << faceCount << " "
           << kdl::str_plural(faceCount, "Brush Face", "Brush Faces");
    break;
  case Action::DeselectAll:
    return "Select None";
    switchDefault();
  }
  return result.str();
}

std::unique_ptr<CommandResult> SelectionCommand::doPerformDo(
  MapDocumentCommandFacade& document)
{
  m_previouslySelectedNodes = document.selectedNodes().nodes();
  m_previouslySelectedFaceRefs = mdl::createRefs(document.selectedBrushFaces());

  switch (m_action)
  {
  case Action::SelectNodes:
    document.performSelect(m_nodes);
    return std::make_unique<CommandResult>(true);
  case Action::SelectFaces:
    return std::make_unique<CommandResult>(
      mdl::resolveAllRefs(m_faceRefs) | kdl::transform([&](const auto& faceHandles) {
        document.performSelect(faceHandles);
      })
      | kdl::transform_error([&](const auto& e) { document.error() << e.msg; })
      | kdl::is_success());
  case Action::SelectAllNodes:
    document.performSelectAllNodes();
    return std::make_unique<CommandResult>(true);
  case Action::SelectAllFaces:
    document.performSelectAllBrushFaces();
    return std::make_unique<CommandResult>(true);
  case Action::ConvertToFaces:
    document.performConvertToBrushFaceSelection();
    return std::make_unique<CommandResult>(true);
  case Action::DeselectNodes:
    document.performDeselect(m_nodes);
    return std::make_unique<CommandResult>(true);
  case Action::DeselectFaces:
    return std::make_unique<CommandResult>(
      mdl::resolveAllRefs(m_faceRefs) | kdl::transform([&](const auto& faceHandles) {
        document.performDeselect(faceHandles);
      })
      | kdl::transform_error([&](const auto& e) { document.error() << e.msg; })
      | kdl::is_success());
  case Action::DeselectAll:
    document.performDeselectAll();
    return std::make_unique<CommandResult>(true);
    switchDefault();
  }
}

std::unique_ptr<CommandResult> SelectionCommand::doPerformUndo(
  MapDocumentCommandFacade& document)
{
  document.performDeselectAll();
  if (!m_previouslySelectedNodes.empty())
  {
    document.performSelect(m_previouslySelectedNodes);
  }
  if (!m_previouslySelectedFaceRefs.empty())
  {
    return std::make_unique<CommandResult>(
      mdl::resolveAllRefs(m_previouslySelectedFaceRefs)
      | kdl::transform(
        [&](const auto& faceHandles) { document.performSelect(faceHandles); })
      | kdl::transform_error([&](const auto& e) { document.error() << e.msg; })
      | kdl::is_success());
  }
  return std::make_unique<CommandResult>(true);
}

} // namespace tb::View
