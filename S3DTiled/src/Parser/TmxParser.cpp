#include "TiledParser.hpp"
#include "../TileSet/CTileSet.hpp"
#include "../TiledMap/CTiledMap.hpp"

#include <S3DTiled/Layer.hpp>

#include <Siv3D/Types.hpp>
#include <Siv3D/Color.hpp>
#include <Siv3D/BinaryReader.hpp>
#include <Siv3D/XMLReader.hpp>
#include <Siv3D/Parse.hpp>
#include <Siv3D/FileSystem.hpp>

namespace
{
    using namespace s3d;
    using namespace s3dTiled;

    Color ParseTiledColor(const String& str)
    {
        bool noneAlpha = str.length() <= 7;
        if (noneAlpha) {
            return Color(str);
        }
        String code = U"#" + str.substr(3, 6);
        Color ret(code);
        ret.setA(Parse<uint32>(U"0x" + str.substr(1, 2)));
        return ret;
    }

    class TmxParser
    {
        FilePath m_parentPath;
        std::shared_ptr<CTiledMap> m_map;
    public:
        TmxParser() = default;

        std::shared_ptr<CTiledMap> parse(s3d::FilePathView path)
        {
            XMLReader root(BinaryReader(path), false);
            if (!root) {
                return nullptr;
            }
            if (FileSystem::IsResource(path)) {
                this->m_parentPath = U"/" +
                    FileSystem::RelativePath(FileSystem::ParentPath(path.substr(1)));
            } else {
                this->m_parentPath = FileSystem::RelativePath(FileSystem::ParentPath(path));
            }

            // map設定
            Size mapSize{
                Parse<int32>(root.attribute(U"width").value_or(U"0")),
                Parse<int32>(root.attribute(U"height").value_or(U"0"))
            };
            Size tileSize{
                Parse<int32>(root.attribute(U"tilewidth").value_or(U"0")),
                Parse<int32>(root.attribute(U"tileheight").value_or(U"0"))
            };
            m_map = std::make_shared<CTiledMap>(mapSize, tileSize);

            if (auto&& col = root.attribute(U"backgroundcolor")) {
                m_map->setBackGroundColor(ParseTiledColor(root.attribute(U"backgroundcolor").value()));
            }

            for (auto elm = root.firstChild(); elm; elm = elm.nextSibling()) {
                if (auto&& layer = this->tryParseLayer(elm)) {
                    m_map->addLayer(Layer(layer));
                } else if (auto&& [firstGId, tileSet] = this->tryTileSet(elm); tileSet) {
                    m_map->addTileSet(firstGId, TileSet(tileSet));
                } else if (elm.name() == U"properties") {
                    m_map->setProps(this->parseProps(elm));
                }

            }

            return m_map;
        }
    private:

#pragma region レイヤーパース
        std::shared_ptr<LayerBase> tryParseLayer(const XMLElement& xml)
        {
            if (xml.name() == U"imagelayer") {
                return this->parseImageLayer(xml);
            } else if (xml.name() == U"layer") {
                return this->parseTileLayer(xml);

            } else if (xml.name() == U"objectgroup") {
                return this->parseObjectGroup(xml);
            } else if (xml.name() == U"group") {
                return this->parseGroupLayer(xml);
            }
            return nullptr;
        }

        void parseLayerCommon(LayerBase* layer, const XMLElement& xml)
        {
            // common
            layer->setId(Parse<s3d::uint32>(xml.attribute(U"id").value_or(U"0")));
            layer->setName(xml.attribute(U"name").value_or(U""));
            layer->setVisible(static_cast<bool>(Parse<s3d::int32>(xml.attribute(U"visible").value_or(U"1"))));
            layer->setOffset({
                Parse<double>(xml.attribute(U"offsetx").value_or(U"0.0")),
                Parse<double>(xml.attribute(U"offsety").value_or(U"0.0"))
                });

            // プロパティは階層が下がるのでここでは取得しない
        }
        std::shared_ptr<LayerBase> parseImageLayer(const XMLElement& xml)
        {
            auto layer = std::make_shared<ImageLayer>();
            this->parseLayerCommon(layer.get(), xml);

            for (auto elm = xml.firstChild(); elm; elm = elm.nextSibling()) {
                if (elm.name() == U"image") {
                    layer->setImagePath(this->m_parentPath + elm.attribute(U"source").value_or(U""));
                } else if (elm.name() == U"properties") {
                    layer->setProps(this->parseProps(elm));
                }
            }
            return layer;
        }

        std::shared_ptr<LayerBase> parseObjectGroup(const XMLElement& xml)
        {
            auto layer = std::make_shared<ObjectGroup>();
            this->parseLayerCommon(layer.get(), xml);

            for (auto elm = xml.firstChild(); elm; elm = elm.nextSibling()) {
                if (elm.name() == U"object") {
                    layer->addObject(this->parseObject(elm));
                } else if (elm.name() == U"properties") {
                    layer->setProps(this->parseProps(elm));
                }
            }
            return layer;
        }


        std::shared_ptr<LayerBase> parseTileLayer(const XMLElement& xml)
        {
            auto layer = std::make_shared<TileLayer>();
            this->parseLayerCommon(layer.get(), xml);

            Grid<GId> grid(
                Parse<uint32>(xml.attribute(U"width").value_or(U"0")),
                Parse<uint32>(xml.attribute(U"height").value_or(U"0"))
            );

            for (auto elm = xml.firstChild(); elm; elm = elm.nextSibling()) {
                if (elm.name() == U"data") {
                    int y = 0;
                    int x = 0;
                    for (auto&& row : elm.text().split(U'\n')) {
                        for (auto&& col : row.split(U',')) {
                            if (col.isEmpty()) {
                                continue;
                            }
                            grid[y][x] = Parse<GId>(col);
                            x++;
                        }
                        x = 0;
                        y++;
                    }
                    layer->setGrid(std::move(grid));
                } else if (elm.name() == U"properties") {
                    layer->setProps(this->parseProps(elm));
                }
            }
            return layer;
        }

        std::shared_ptr<LayerBase> parseGroupLayer(const XMLElement& xml)
        {
            auto layer = std::make_shared<GroupLayer>();
            this->parseLayerCommon(layer.get(), xml);

            for (auto elm = xml.firstChild(); elm; elm = elm.nextSibling()) {
                if (auto&& child = this->tryParseLayer(elm)) {
                    layer->addLayer(Layer(child));
                } else if (elm.name() == U"properties") {
                    layer->setProps(this->parseProps(elm));
                }
            }
            return layer;
        }
#pragma endregion

        Properties parseProps(const XMLElement& xml)
        {
            Properties ret;

            for (auto elm = xml.firstChild(); elm; elm = elm.nextSibling()) {
                if (elm.name() == U"property") {
                    ret[elm.attribute(U"name").value_or(U"")] = this->parseProp(elm);
                }
            }
            return ret;
        }

        Property parseProp(const XMLElement& xml)
        {
            const String& type = xml.attribute(U"type").value_or(U"string");
            const String& value = xml.attribute(U"value").value_or(U"");

            if (type == U"bool") {
                return Parse<bool>(value);
            } else if (type == U"color") {
                return ParseTiledColor(value);
            } else if (type == U"float") {
                return Parse<double>(value);
            } else if (type == U"int") {
                return Parse<int32>(value);
            } else {
                return type == U"file" ? this->m_parentPath + value : value;
            }
        }
        bool tryParseShape(const XMLElement& elm, Shape& shape, const Vec2& size)
        {
            if (elm.name() == U"ellipse") {
                shape = Ellipse({ 0,0 }, size);
                return true;
            }
            if (elm.name() == U"polygon") {
                Array<Vec2> pos;
                for (auto&& row : elm.attribute(U"points")->split(L' ')) {
                    const auto& s = row.split(U',');
                    pos.emplace_back(
                        Parse<double>(s[0]),
                        Parse<double>(s[1])
                    );
                }
                shape = Polygon(pos);
                return true;
            }
            if (elm.name() == U"polyline") {
                Array<Vec2> pos;
                for (auto&& row : elm.attribute(U"points")->split(U' ')) {
                    const auto& s = row.split(U',');
                    pos.emplace_back(
                        Parse<double>(s[0]),
                        Parse<double>(s[1])
                    );
                }
                shape = LineString(pos);
                return true;
            }
            return false;
        }


        Object parseObject(const XMLElement& xml)
        {
            Object obj;
            Properties props;

            obj.id = Parse<uint32>(xml.attribute(U"id").value_or(U"0"));
            obj.name = xml.attribute(U"name").value_or(U"");
            obj.type = xml.attribute(U"type").value_or(U"");
            obj.pos = {
                Parse<double>(xml.attribute(U"x").value_or(U"0")),
                Parse<double>(xml.attribute(U"y").value_or(U"0"))
            };
            obj.rotation = Parse<double>(xml.attribute(U"rotation").value_or(U"0"));
            if (auto&& gId = xml.attribute(U"gid")) {
                auto rawGId = Parse<GId>(xml.attribute(U"gid").value());

                constexpr GId mirrorFlag = 0x80000000;
                constexpr GId flipFlag = 0x40000000;

                obj.isMirrored = rawGId & mirrorFlag;
                obj.isFliped = rawGId & flipFlag;

                auto fixedGId = rawGId & ~(mirrorFlag | flipFlag);
                obj.gId = fixedGId;
                // tile側のプロパティを引き継ぐ
                props = m_map->getTileProperties(*obj.gId);
            }
            Vec2 size{
                Parse<double>(xml.attribute(U"width").value_or(U"0")),
                Parse<double>(xml.attribute(U"height").value_or(U"0"))
            };


            //shape
            bool isRect = true;
            for (auto elm = xml.firstChild(); elm; elm = elm.nextSibling()) {
                if (this->tryParseShape(elm, obj.shape, size)) {
                    isRect = false;
                } else if (elm.name() == U"properties") {
                    for (auto&& [key, value] : this->parseProps(elm)) {
                        props[key] = value;
                    }
                }
            }
            if (isRect) {
                obj.shape = RectF({ 0,0 }, size);
            }
            obj.setProps(std::move(props));
            return obj;
        }

#pragma region タイルセット
        std::pair<GId, std::shared_ptr<CTileSet>> tryTileSet(const XMLElement& xml)
        {
            if (xml.name() != U"tileset") {
                return { 0, nullptr };
            }
            auto firstGid = Parse<GId>(xml.attribute(U"firstgid").value_or(U"0"));
            if (xml.attribute(U"columns").value_or(U"0") == U"0") {
                return { firstGid, this->parseVariousTileSet(xml) };
            } else {
                return { firstGid, this->parseUniformTileSet(xml) };
            }
        }

        void parseTileSetCommon(CTileSet* tileSet, const XMLElement& xml)
        {
            tileSet->setTileCount(Parse<uint32>(xml.attribute(U"tilecount").value_or(U"0")));
            tileSet->setColumns(Parse<uint32>(xml.attribute(U"columns").value_or(U"0")));

            // animationは階層が下がるのでここではやらない
        }

        Animation parseAnimation(const XMLElement& xml)
        {
            Animation ret;

            for (auto elm = xml.firstChild(); elm; elm = elm.nextSibling()) {
                if (elm.name() != U"frame") {
                    continue;
                }
                int32 duration = Parse<int32>(elm.attribute(U"duration").value_or(U"0"));
                TileId tileId = Parse<TileId>(elm.attribute(U"tileid").value_or(U"0"));
                ret.addFrame(tileId, duration);
            }
            return ret;
        }

        void tryParseTileInfo(Tile* tile, const XMLElement& xml)
        {
            for (auto elm = xml.firstChild(); elm; elm = elm.nextSibling()) {
                if (elm.name() == U"animation") {
                    tile->animation = this->parseAnimation(elm);
                } else if (elm.name() == U"properties") {
                    tile->props = this->parseProps(elm);
                }
            }
        }
        std::shared_ptr<CTileSet> parseUniformTileSet(const XMLElement& xml)
        {
            auto tileSet = std::make_shared<CTileSet>();
            this->parseTileSetCommon(tileSet.get(), xml);

            s3d::Size tileSize{
                Parse<int32>(xml.attribute(U"tilewidth").value_or(U"0")),
                Parse<int32>(xml.attribute(U"tileheight").value_or(U"0"))
            };

            // 先に画像パスを取得
            s3d::String image;
            for (auto elm = xml.firstChild(); elm; elm = elm.nextSibling()) {
                if (elm.name() == U"image") {
                    image = this->m_parentPath + elm.attribute(U"source").value_or(U"");
                    break;
                }
            }
            // タイル登録
            {
                auto tileCount = tileSet->getTileCount();
                auto columns = tileSet->getColumns();
                for (TileId tileId = 0; tileId < tileCount; ++tileId) {
                    uint32 x = tileId % columns;
                    uint32 y = tileId / columns;

                    tileSet->add(Tile{
                        .tileId = tileId,
                        .image = image,
                        .offset = { tileSize.x * x, tileSize.y * y },
                        .size = tileSize,
                        });
                }
            }
            // その他
            for (auto elm = xml.firstChild(); elm; elm = elm.nextSibling()) {
                if (elm.name() == U"tile") {
                    TileId tileId = Parse<TileId>(elm.attribute(U"id").value_or(U"0"));
                    this->tryParseTileInfo(&tileSet->find(tileId), elm);
                }
            }
            return tileSet;
        }

        std::shared_ptr<CTileSet> parseVariousTileSet(const XMLElement& xml)
        {
            auto tileSet = std::make_unique<CTileSet>();
            this->parseTileSetCommon(tileSet.get(), xml);

            auto tileCount = tileSet->getTileCount();

            for (auto elm = xml.firstChild(); elm; elm = elm.nextSibling()) {
                if (elm.name() == U"tile") {
                    TileId tileId = Parse<TileId>(elm.attribute(U"id").value_or(U"0"));

                    // タイル数よりIdのほうが多いときある
                    tileCount = s3d::Max(tileCount, tileId + 1u);
                    Tile tile{};
                    tile.tileId = tileId;
                    this->tryParseTileInfo(&tile, elm);

                    for (auto image = elm.firstChild(); image; image = image.nextSibling()) {
                        if (image.name() == U"image") {
                            tile.size = Vec2{
                                Parse<double>(image.attribute(U"width").value_or(U"0")),
                                Parse<double>(image.attribute(U"height").value_or(U"0"))
                            };
                            tile.image = this->m_parentPath + image.attribute(U"source").value_or(U"");
                            tileSet->add(std::move(tile));
                            break;
                        }
                    }
                }
            }
            tileSet->setTileCount(tileCount);
            return tileSet;
        }
#pragma endregion

    };
}  // namespace

namespace s3dTiled
{
    std::shared_ptr<CTiledMap> ParseTmx(s3d::FilePathView path)
    {
        TmxParser parser;
        return parser.parse(path);
    }
}  // namespace s3dTiled