#include "mvFloatVectValue.h"
#include "mvTypeBases.h"
#include <utility>
#include "mvApp.h"
#include "mvModule_Core.h"
#include <string>
#include "mvItemRegistry.h"

namespace Marvel {

    void mvFloatVectValue::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        mvPythonParser parser(mvPyDataType::UUID, "Undocumented", { "Widgets", "Values"});
        mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_SOURCE)
        );

        parser.addArg<mvPyDataType::FloatList>("default_value", mvArgType::KEYWORD_ARG, "()");

        parser.finalize();

        parsers->insert({ s_command, parser });
    }

    mvFloatVectValue::mvFloatVectValue(mvUUID uuid)
        : mvFloatVectPtrBase(uuid)
    {
    }

}
