#ifndef INCLUDE_MATRIZ_HPP_
#define INCLUDE_MATRIZ_HPP_

#include <stdexcept>
#include <vector>

// undefine to disable range checking
#define RANGE_CHECK

namespace Matriz{
   template<class T>
   class Matriz {
   	public:
         const unsigned linha;
         const unsigned coluna;
    protected:
         std::vector<T> elementos;
         // range check function for matrix access
         void range_check( unsigned i, unsigned j ) const;
    public:
         T& operator()( unsigned i, unsigned j ) {
            #ifdef RANGE_CHECK
            range_check(i,j);
            #endif
            return elementos[i*coluna+j];
         }

         const T& operator()( unsigned i, unsigned j ) const {
            #ifdef RANGE_CHECK
            range_check(i,j);
            #endif
            return elementos[i*coluna+j];
         }
         Matriz( unsigned rows, unsigned cols, std::vector<T> elements = 0 );
         Matriz( const Matriz<T>& );
         ~Matriz();

         // matrix multiplication
         Matriz<T> operator*( Matriz<T>& M ) {
            Matriz<double> v(linha, M.coluna, elementos);
            for (unsigned int i = 0; i < linha; i++) {
               for (unsigned int j = 0; j < M.coluna; j++) {
                  double soma = 0;
                  for (unsigned int k = 0; k < coluna; k++){
                     soma +=  (*this)(i,k)*M(k,j);
                  }
                  v(i,j)=soma;
               }
            }
            return v;
         }
         Matriz<T> getrow( unsigned j ) const;
         Matriz<T> getcol( unsigned i ) const;

         Matriz<T> transpose() const;
         Matriz<T> operator~() const {
         return transpose();
      }
   };


   template<class T>
   Matriz<T>::Matriz( unsigned rows, unsigned cols, std::vector<T> elements ) : linha(rows), coluna(cols), elementos(elements){
      if( rows == 0 || cols == 0 )
      throw std::range_error("attempt to create a degenerate matrix");
      // initialze from array
      if(!elements.empty())
      elementos = elements;
      else{
         elementos.reserve(linha*coluna);
         for (int i = 0; i < linha*coluna; i++){
            elementos.push_back(0.0);
         }
      }
   };

   template<class T>
   Matriz<T>::Matriz( const Matriz<T>& cp ) : linha(cp.linha), coluna(cp.coluna), elementos(cp.elementos){
   }

   template<class T>
   Matriz<T>::~Matriz(){
   }

   template<class T>
   void Matriz<T>::range_check( unsigned i, unsigned j ) const {
      if( linha <= i )
      throw std::range_error("matrix access row out of range");
      if( coluna <= j )
      throw std::range_error("matrix access col out of range");
   }
}
#endif /* INCLUDE_MATRIZ_HPP_ */
