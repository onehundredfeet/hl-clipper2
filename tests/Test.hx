// This file is written entirely in the Haxe programming language
package ;

import hl.Bytes;
import haxe.unit.TestRunner;
import haxe.unit.TestCase;

import clip.Native;

//write testsuit
class TestSuite extends haxe.unit.TestCase {
    public function new() {
        super();
    }

    public function testBasic() {
        var x = new PathsD();
        var coords = new Array<Float>();
        coords.push(0.0);
        coords.push(0.0);
        coords.push(1.0);
        coords.push(0.0);
        coords.push(1.0);
        coords.push(1.0);
        
        x.addPolygonD(Bytes.getArray(coords), 3 );

        //write test
        assertEquals(1, 1);
    }

    public function testMultiple() {
        var x = new PathsD();
        var coords = new Array<Float>();
        coords.push(0.0);
        coords.push(0.0);
        coords.push(1.0);
        coords.push(0.0);
        coords.push(1.0);
        coords.push(1.0);

        coords.push(0.0);
        coords.push(0.0);
        coords.push(1.0);
        coords.push(1.1);
        coords.push(0.0);
        coords.push(1.0);
        
        var counts = new Array<Int>();
        counts.push(3);
        counts.push(3);

        x.addPolygonsD(Bytes.getArray(coords),Bytes.getArray(counts), 2 );

        //write test
        assertEquals(1, 1);
    }

    public function testUnion() {
        var x = new PathsD();
        var coords = new Array<Float>();
        coords.push(0.0);
        coords.push(0.0);
        coords.push(1.0);
        coords.push(0.0);
        coords.push(1.0);
        coords.push(1.0);

        coords.push(0.0);
        coords.push(0.0);
        coords.push(1.0);
        coords.push(1.0);
        coords.push(0.0);
        coords.push(1.0);
        
        var counts = new Array<Int>();
        counts.push(3);
        counts.push(3);

        x.addPolygonsD(Bytes.getArray(coords),Bytes.getArray(counts), 2 );

        var y = x.unionAll();

        //write test
        assertEquals(1, y.numPolygons());

        var counts = new Array<Int>();
        counts.resize( y.numPolygons() );
        var totalCount = y.polygonVertCounts(Bytes.getArray(counts));

        assertEquals(4, totalCount);
        assertEquals(4, counts[0]);

        var unionCoords = new Array<Float>();
        unionCoords.resize( totalCount * 2 );

        y.getAllCoordinates( Bytes.getArray(unionCoords) );

        var numCoords = Std.int(unionCoords.length / 2);
        trace('Coordinates');
        for (i in 0...numCoords) {
            trace(' x ${unionCoords[i*2]} y ${unionCoords[i*2+1]}');
        }
    }
}

class Test {
    static function main() {
        var runner = new TestRunner();
        runner.add(new TestSuite());
        runner.run();
    }
}
