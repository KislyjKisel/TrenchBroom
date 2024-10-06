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

#include "SwapNodeContentsCommand.h"

#include "View/MapDocumentCommandFacade.h"
#include "mdl/Node.h"

#include "kdl/vector_utils.h"

namespace tb::View
{

SwapNodeContentsCommand::SwapNodeContentsCommand(
  std::string name, std::vector<std::pair<mdl::Node*, mdl::NodeContents>> nodes)
  : UpdateLinkedGroupsCommandBase{std::move(name), true}
  , m_nodes{std::move(nodes)}
{
}

SwapNodeContentsCommand::~SwapNodeContentsCommand() = default;

std::unique_ptr<CommandResult> SwapNodeContentsCommand::doPerformDo(
  MapDocumentCommandFacade& document)
{
  document.performSwapNodeContents(m_nodes);
  return std::make_unique<CommandResult>(true);
}

std::unique_ptr<CommandResult> SwapNodeContentsCommand::doPerformUndo(
  MapDocumentCommandFacade& document)
{
  document.performSwapNodeContents(m_nodes);
  return std::make_unique<CommandResult>(true);
}

bool SwapNodeContentsCommand::doCollateWith(UndoableCommand& command)
{
  if (auto* other = dynamic_cast<SwapNodeContentsCommand*>(&command))
  {
    auto myNodes =
      kdl::vec_transform(m_nodes, [](const auto& pair) { return pair.first; });
    auto theirNodes =
      kdl::vec_transform(other->m_nodes, [](const auto& pair) { return pair.first; });

    kdl::vec_sort(myNodes);
    kdl::vec_sort(theirNodes);

    return myNodes == theirNodes;
  }

  return false;
}

} // namespace tb::View
