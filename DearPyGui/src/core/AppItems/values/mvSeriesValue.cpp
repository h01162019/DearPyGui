#include "mvSeriesValue.h"
#include "mvTypeBases.h"
#include <utility>
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>
#include "mvItemRegistry.h"

namespace Marvel {

    void mvSeriesValue::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        mvPythonParser parser(mvPyDataType::UUID, "Undocumented", { "Widgets", "Values"});
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_SOURCE)
        );

        parser.addArg<mvPyDataType::DoubleList>("default_value", mvArgType::KEYWORD_ARG, "()");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvSeriesValue::mvSeriesValue(mvUUID uuid)
        : mvSeriesBase(uuid)
    {
    }

}
