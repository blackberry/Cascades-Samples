/* Copyright (c) 2012 Research In Motion Limited.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import bb.cascades 1.2
import "QuotesListPage"

NavigationPane {
    id: nav

    // A reference to to the Page for presenting the page is kept while showing it
    // in order to update its data when deleting and editing the quotes.
    property variant quotePage;
    
    QuotesListPage {
        id: quotesListPage
    
        onShowQuotesPage: {
            // Create the content page and push it on top to drill down to it.
            quotePage = quotePageDefinition.createObject();
            quotePage.quoteData = selectedData;       
            nav.push(quotePage);
            
            quotePage.deleteQuote.connect(quotesListPage.deleteSelectedQuote);
            quotePage.updateQuote.connect(quotesListPage.updateSelectedQuote);            
            quotePage.back.connect(nav.pop);
            
        }
        
        onSelectedDataChanged :{
            if(quotePage != undefined) {
                quotePage.quoteData = selectedData;
            }
        }
        
        onListEmpty: {
            if (nav.top == quotePage) {
                nav.pop();
            }
        }
    }
        
    attachedObjects: [
        ComponentDefinition {
            // Definition used for creating a Content Page to which the
            // user can drill down to read the quote.
            id: quotePageDefinition
            source: "QuotePage/QuotePage.qml"
        }
    ]
    
    onPopTransitionEnded: {
        // The only occurrence of a pop transition that ends is when the Page
        // with the text in a bubble is pushed by back navigation, this Page
        // is created each time the a new quote is selected in the list so
        // in order to avoid memory leaks it is destroyed here.
        if (quotePage == page) {
            page.destroy();
        }
    }
}// NavigationPane
