# i n c l u d e   " c h a n n e l _ s e l e c t . h "  
 # i n c l u d e   " t r a n s f e r _ u s r d e s i g n . h "  
 # i n c l u d e   " w e c h a t _ u s r d e s i g n . h "  
 # i n c l u d e   " a p p _ w e c h a t _ c o m m o n . h "  
 # i n c l u d e   " u s r _ d a t a . h "  
 # i n c l u d e   " d e b u g . h "  
 # i n c l u d e   < s t r i n g . h >  
  
 c o m m u n i c a t i o n _ s t a t u e _ s t   g _ c o m m u n i c a t i o n _ s t a t u e   =   { 0 } ;  
  
 v o i d   u s r _ s e t _ a p p _ t y p e ( a p p _ e n u m   t y p e )  
 {  
 	 g _ c o m m u n i c a t i o n _ s t a t u e . a p p _ t y p e   | =   t y p e ;  
 }  
  
 u i n t 3 2 _ t   a p p _ s e n d _ d a t a ( u i n t 8 _ t   * d a t a , u i n t 1 6 _ t   l e n g t h )  
 {  
 	 u i n t 3 2 _ t   e r r o r ;  
  
 	 i f ( g _ c o m m u n i c a t i o n _ s t a t u e . a p p _ t y p e   = =   W E C H A T _ A P P )  
 	 { 	  
 	 	 e r r o r   =   w e c h a t _ s e n d _ d a t a ( d a t a ,   l e n g t h , W E C H A T _ I N D I C A T E _ C H A N N E L , 0 ) ;  
 	 }  
 	 e l s e   i f ( g _ c o m m u n i c a t i o n _ s t a t u e . a p p _ t y p e   = =   L I F E S E N S E _ A P P )  
 	 { 	  
 	 	 e r r o r   =   t r a n s f e r _ s e n d _ d a t a ( d a t a ,   l e n g t h , T R A N S _ I N D I C A T E _ C H A N N E L , 0 ) ;  
 	 }  
 	 r e t u r n   e r r o r ;  
 }  
  
 u i n t 3 2 _ t   u s r _ s e n d _ d a t a ( u i n t 8 _ t   * d a t a , u i n t 1 6 _ t   l e n g t h )  
 {  
 	 u i n t 3 2 _ t   e r r o r ;  
 	 u i n t 8 _ t   s e n d _ b u f f e r [ 2 2 0 ] ;  
 	 W e C h a t P a c k H e a d e r   w e c h a t _ h e a d ;  
 	 S e n d D a t a R e q u e s t _ t 	   S e n d D a t a R e q u e s t ;  
 	 u i n t 8 _ t   l e n , o u t _ l e n , * p _ i n _ d a t a , * p _ o u t _ d a t a ;  
  
 	 t r a n s _ h e a d e r _ s t 	   t r a n s _ h e a d e r ;  
 	  
 	 m e m s e t ( s e n d _ b u f f e r ,   0 ,   s i z e o f ( s e n d _ b u f f e r ) ) ;  
  
 	 i f ( g _ c o m m u n i c a t i o n _ s t a t u e . a p p _ t y p e   = =   L I F E S E N S E _ A P P )  
 	 { 	  
 	 	 p _ i n _ d a t a   	 =   d a t a ;  
 	 	  
 	 	 t r a n s _ h e a d e r . u s T x D a t a T y p e   	 	 =   0 ;  
 	         t r a n s _ h e a d e r . u s T x D a t a P a c k S e q 	 =   0 x 0 0 0 1 ; 	 / / � � � � � �  
 	         t r a n s _ h e a d e r . u s L e n g t h   	 	 	 =   l e n g t h ;  
 	         t r a n s _ h e a d e r . u s T x D a t a F r a m e S e q 	 =   0 x 0 1 ;   	 / / � � � � � �  
 	          
 	         o u t _ l e n   =   a p p _ a d d _ p a c k _ h e a d ( t r a n s _ h e a d e r , p _ i n _ d a t a , s e n d _ b u f f e r ,   0 ) ;  
 	 }  
 	 e l s e   i f ( g _ c o m m u n i c a t i o n _ s t a t u e . a p p _ t y p e   = =   W E C H A T _ A P P )  
 	 { 	  
 	 	 w e c h a t _ h e a d . u c M a g i c N u m b e r =   W E C H A T _ P A C K _ H E A D _ M A G I C N U M ;               	 / / � � � � � � m a g i c   n u m b e r � � � � � � � � � � � � � � � �  
 	 	 w e c h a t _ h e a d . u c V e r s i o n 	 =   W E C H A T _ P A C K _ H E A D _ V E R S I O M ; 	 	 	 / / � � � � � � v e r s i o n r � � � � � � � � � � � � � � � � 	  
 	 	 w e c h a t _ h e a d . u s L e n g t h   	 =   0 ;  
 	 	 w e c h a t _ h e a d . u s C m d I D   	 	 =   W E C H A T _ C M D I D _ R E Q _ U T C ;  
 	 	 w e c h a t _ h e a d . u s T x D a t a P a c k S e q u e n c e   =   u s T x W e C h a t P a c k S e q + + ; 	 	 / /   > =   0 x 0 0 0 3 ;  
 	  
 	 	 S e n d D a t a R e q u e s t . B a s e R e q u e s t   = 0 x 0 0 ;  
  
 	 	 S e n d D a t a R e q u e s t . D a t a   =   d a t a ;  
  
 	 	 p _ o u t _ d a t a   	 =   u c D a t a A f t e r P a c k ;  
 	 	  
 	 	 p _ i n _ d a t a   	 =   & S e n d D a t a R e q u e s t . B a s e R e q u e s t ;  
 	 	 o u t _ l e n 	 	 =   P a c k D a t a T y p e ( D A T A _ B A S E _ R E Q U E S T _ F I E L D ,   L e n g t h _ d e l i m i t ,   p _ i n _ d a t a ,   D A T A _ B A S E _ R E Q U E S T _ L E N G T H ,   p _ o u t _ d a t a ) ;  
 	 	 p _ o u t _ d a t a   	 + =   o u t _ l e n ;  
 	 	 l e n   	 	 =   o u t _ l e n ;  
  
 	 	 p _ i n _ d a t a   	 =   S e n d D a t a R e q u e s t . D a t a ;  
 	 	 o u t _ l e n 	 	 =   P a c k D a t a T y p e ( D A T A _ D A T A _ F I E L D ,   L e n g t h _ d e l i m i t ,   p _ i n _ d a t a ,   l e n g t h ,   p _ o u t _ d a t a ) ;  
 	 	 l e n   	 	 + =   o u t _ l e n ;  
  
 	 	 w e c h a t _ h e a d . u s L e n g t h   =   l e n   +   W E C H A T _ P A C K E T _ H E A D _ L E N G T H ;  
 	 	 o u t _ l e n   =   a p p _ a d d _ w e c h a t _ h e a d ( w e c h a t _ h e a d ,   u c D a t a A f t e r P a c k ,   s e n d _ b u f f e r ) ;  
 	 }  
  
 	 i f ( g _ c o m m u n i c a t i o n _ s t a t u e . a p p _ t y p e   = =   L I F E S E N S E _ A P P )  
 	 { 	  
 	 	 e r r o r   =   t r a n s f e r _ s e n d _ d a t a ( s e n d _ b u f f e r ,   o u t _ l e n , T R A N S _ I N D I C A T E _ C H A N N E L , 0 ) ;  
 	 }  
 	 e l s e   i f ( g _ c o m m u n i c a t i o n _ s t a t u e . a p p _ t y p e   = =   W E C H A T _ A P P )  
 	 { 	 	 	  
 	 	 e r r o r   =   w e c h a t _ s e n d _ d a t a ( s e n d _ b u f f e r ,   o u t _ l e n , W E C H A T _ I N D I C A T E _ C H A N N E L , 0 ) ;  
 	 }  
 	  
 	 r e t u r n   e r r o r ;  
 }  
  
 u i n t 3 2 _ t   a p p _ a d d _ h e a p _ s e n d _ d a t a ( u i n t 8 _ t   d a t a _ t y p e , u i n t 8 _ t   d a t a _ i d , u i n t 8 _ t   * d a t a , u i n t 1 6 _ t   l e n g t h )  
 {  
 	 r e t u r n   0 ;  
 }  
  
 