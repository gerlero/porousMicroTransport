#include "dictionaries.H"

const Foam::dictionary&
Foam::Pmt::dictionaries::subOrNullDictRef(const dictionary& dict, const word& keyword)
{
   if (const auto* sub = dict.findDict(keyword))
   {
        return *sub;
   }

   return dictionary::null;
}
