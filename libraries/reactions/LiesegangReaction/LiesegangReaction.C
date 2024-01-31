#include "LiesegangReaction.H"

#include <className.H>
#include <addToRunTimeSelectionTable.H>
#include <messageStream.H>

namespace Foam
{
namespace Pmt
{
defineTypeNameAndDebug(LiesegangReaction, 0);
addToRunTimeSelectionTable(reaction, LiesegangReaction, dictionary);
}
}

Foam::Pmt::LiesegangReaction::LiesegangReaction
(
    const basicMultiComponentMixture& composition,
    const dictionary& reactionDict
)
:
    reaction{reactionDict.get<string>("reaction"), composition.species()},
    composition_{composition},
    kf1_scalar{"kf1", kfDimensions(YDimensions(composition)), reactionDict},
    kf2_scalar{"kf2", dimVol/(dimTime*dimMoles), reactionDict},
    kr_scalar{dimensionedScalar::getOrDefault("kr", reactionDict, krDimensions(YDimensions(composition)), Zero)},
    cs{"cs", dimMoles/dimVolume, reactionDict},
    rho{"rho", dimMoles/dimVolume, reactionDict},
    cradius{"cradius", dimLength, reactionDict},
    cs_field{volScalarField::New
                (
                    "binary",
                    composition_.Y().first().mesh(),
                    dimensionedScalar(1).value()
    )},
    heaviField{volScalarField::New
                (
                    "binary",
                    composition_.Y().first().mesh(),
                    dimensionedScalar(1).value()
    )},
    cellSizes{volScalarField::New
                (
                    "binary",
                    composition_.Y().first().mesh(),
                    dimensionedScalar(1).value()
    )},
    binary{volScalarField::New
                (
                    "binary",
                    composition_.Y().first().mesh(),
                    dimensionedScalar(1).value()
    )},
    inRadius{composition_.Y().first().mesh().C().size(),List<label>()}
{
    Info<< "Reaction " << reactionDict.name() << nl
        << "    "; write(Info, composition.species()); Info<< nl
        << "    kf1 = " << kf1_scalar.value() << nl
        << "    kf2 = " << kf2_scalar.value() << endl;
    
    if (kr_scalar.value() != 0)
    {
        Info<< "    kr = " << kr_scalar.value() << endl;
    }

    if(lhs().size() != 1 || lhs().first().exponent != 1 || lhs().first().stoichCoeff != 1 ||
        rhs().size() != 1 || rhs().first().exponent != 1 || rhs().first().stoichCoeff != 1)
    {
        FatalErrorInFunction
            << "Unsupported Liesegang reaction" << nl
            << abort(FatalError);
    }

    //binary = List<bool>(composition_.Y().first().size(), Zero);
    binary = volScalarField::New
                (
                    "binary",
                    composition_.Y().first().mesh(),
                    dimensionedScalar(1).value()
                );

    calcCellsInRadius();
    updateCSfield(cs);
}

Foam::label Foam::Pmt::LiesegangReaction::find(const List<label>& list,label elem){
    forAll(list, index){
        if(list[index] == elem){
            return index;
        }
    }
    return -1;
}

void Foam::Pmt::LiesegangReaction::updateCSfield(dimensionedScalar cs_scalar){
        cs_field = volScalarField::New
                (
                    "cs",
                    composition_.Y().first().mesh(),
                    cs_scalar.value()
                );
}

void Foam::Pmt::LiesegangReaction::calcCellsInRadius(){

    auto& mesh = composition_.Y().first().mesh();
    const auto& centers_pos = mesh.C();
    const auto& owner = mesh.owner();
    const auto& neighbour = mesh.neighbour();
    List<List<label>> adjacency(centers_pos.size(),List<label>());
    forAll(owner,faceid){
        const label from = owner[faceid];
        const label to = neighbour[faceid];
        if(mag(centers_pos[from]-centers_pos[to])<= cradius.value()){
            adjacency[from].append(to);
            adjacency[to].append(from);
        }
    }
    FIFOStack<label> queue;//Linked List cause List doesnt have pop
    forAll(centers_pos,center){
        queue.clear();
        queue.push(center);//BFS 
        while(queue.size() > 0){
            label to_expand = queue.pop();
            forAll(adjacency[to_expand],adjidx){
                label adj = adjacency[to_expand][adjidx];
                if(adj != center &&
                   find(inRadius[center],adj) == -1 &&      //this won't work if inRadius is initialized null
                   mag(centers_pos[center]-centers_pos[adj]) <= cradius.value())
                {
                    inRadius[center].append(adj);
                    queue.push(adj);
                }
            }
        }
    }
}


Foam::tmp<Foam::volScalarField> Foam::Pmt::LiesegangReaction::kf()
{

    const auto& C = composition_.Y(lhs().first().index);
    const auto& D = composition_.Y(rhs().first().index);
    const auto& cField = C.internalField();
    const auto& dField = D.internalField();
    
    forAll(dField, cell){
            binary[cell] = dField[cell] < rho.value();
            
            bool saturation = true;
            forAll(inRadius[cell],idx){
                label target = inRadius[cell][idx];
                saturation = saturation && (dField[target] < rho.value());
            }
            cs_field[cell] *= saturation;
        }

    heaviside2InternalField(cField, cs_field.ref());

    return volScalarField::New("kf", (kf1_scalar*heaviField+kf2_scalar*D)*binary);      //only for reactions of C->D type

}

Foam::tmp<Foam::volScalarField> Foam::Pmt::LiesegangReaction::kr()
{
    return volScalarField::New("kr", composition_.Y().first().mesh(), kr_scalar);
}


// * * * * * * * * * * * * * * * * AUX * * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
void printField(const Foam::GeometricField<Type,PatchField,GeoMesh>& dfield){
	const auto& field = dfield.internalField();
    forAll(field, index){        
		if(index % 100 == 0){
	        //Foam::Info << Foam::endl;
		}
		Foam::Info << " " << field[index];
	}
	Foam::Info << Foam::endl;
}

double lerp(double l,double r,double p){
	return l*(1-p)+r*p;
}

double sigLin(double x){
	double y = x;
	y += 0.005;
	y *= (0.8/0.005);
	y += 0.1;
	return (x < -0.005)*0+(x >= -0.005 && x <= 0)*y+(x > 0)*1;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::Pmt::LiesegangReaction::heaviside2InternalField
(
    const DimensionedField<scalar, Foam::volMesh>& cField,
    const DimensionedField<scalar, Foam::volMesh>& csField
)
{

	auto& hIntfield = heaviField.ref();
    forAll(cField,cell){
        hIntfield[cell] = sigLin(cField[cell]-csField[cell]);
    }

}

