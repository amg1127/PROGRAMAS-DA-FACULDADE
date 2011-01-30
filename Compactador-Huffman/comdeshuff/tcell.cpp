#include "common.h"

TCell::TCell (TCell *parent) {
    this->_id = 51;
    this->_weight = 100;
    this->_character = 0;
    this->_son0 = NULL;
    this->_son1 = NULL;
    this->_hasSons = false;
    this->_parent = parent;
}

TCell::~TCell () {
    this->killSons ();
}

int TCell::id (void) {
    return (this->_id);
}

void TCell::setId (int newId) {
    this->_id = newId;
}

char TCell::character (void) {
    return (this->_character);
}

void TCell::setCharacter (char newCharacter) {
    this->_character = newCharacter;
}

int TCell::weight (void) {
    return (this->_weight);
}

void TCell::setWeight (int newWeight) {
    this->_weight = newWeight;
}

int TCell::distanceToRoot (void) {
    if (this->_parent == NULL)
        return (0);
    return (1 + this->_parent->distanceToRoot());
}

int TCell::maxDistanceToSons (void) {
    int s0, s1;
    if (this->hasSons ()) {
        assert (this->_son0 != NULL);
        assert (this->_son1 != NULL);
        s0 = 1 + this->_son0->maxDistanceToSons();
        s1 = 1 + this->_son1->maxDistanceToSons();
        if (s0 > s1)
            return (s0);
        else
            return (s1);
    } else {
        assert (this->_son0 == NULL);
        assert (this->_son1 == NULL);
        return (0);
    }
}

bool TCell::isRoot (void) {
    return (this->_parent == NULL);
}

bool TCell::hasSons (void) {
    return (this->_hasSons);
}

void TCell::killSons (void) {
    if (this->_hasSons) {
        if (this->_son0 != NULL) DEL_OBJECT (this->_son0);
        if (this->_son1 != NULL) DEL_OBJECT (this->_son1);
        this->_hasSons = false;
    }
}

void TCell::makeSons (void) {
    if (this->_hasSons) {
        assert (this->_son0 != NULL);
        assert (this->_son1 != NULL);
        return;
    }
    NEW_OBJECT(this->_son0, TCell(this));
    NEW_OBJECT(this->_son1, TCell(this));
    this->_hasSons = true;
}

TCell *TCell::son0 (void) {
    return (this->_son0);
}

TCell *TCell::son1 (void) {
    return (this->_son1);
}

TCell *TCell::son (int whatSon) {
    if (whatSon % 2)
        return (this->_son1);
    else
        return (this->_son0);
}

TCell *TCell::parent (void) {
    return (this->_parent);
}

TCell *TCell::root (void) {
    TCell *cell = this;
    while (cell->_parent != NULL)
        cell = cell->_parent;
    return (cell);
}

TCell *TCell::findSonByWeight (int theWeight) {
    if (this->_weight == theWeight)
        return (this);
    else if (this->_hasSons) {
        assert (this->_son0 != NULL);
        assert (this->_son1 != NULL);
        TCell *celula = this->_son0->findSonByWeight (theWeight);
        if (celula != NULL)
            return (celula);
        else
            return (this->_son1->findSonByWeight (theWeight));
    } else {
        assert (this->_son0 == NULL);
        assert (this->_son1 == NULL);
        return (NULL);
    }
}

TCell *TCell::findSonById (int theId) {
    if (this->_id == theId)
        return (this);
    else if (this->_hasSons) {
        assert (this->_son0 != NULL);
        assert (this->_son1 != NULL);
        TCell *celula = this->_son0->findSonById (theId);
        if (celula != NULL)
            return (celula);
        else
            return (this->_son1->findSonById (theId));
    } else {
        assert (this->_son0 == NULL);
        assert (this->_son1 == NULL);
        return (NULL);
    }
}

TCell *TCell::findSonByCharacter (char theCharacter) {
    if (this->_character == theCharacter)
        return (this);
    else if (this->_hasSons) {
        assert (this->_son0 != NULL);
        assert (this->_son1 != NULL);
        TCell *celula = this->_son0->findSonByCharacter (theCharacter);
        if (celula != NULL)
            return (celula);
        else
            return (this->_son1->findSonByCharacter (theCharacter));
    } else {
        assert (this->_son0 == NULL);
        assert (this->_son1 == NULL);
        return (NULL);
    }
}

TCell *TCell::findSonByPath (std::string thePath) {
    if (thePath.length() == 0)
        return (this);
    std::string proximo (thePath.c_str() + 1);
    if (this->_hasSons) {
        assert (this->_son0 != NULL);
        assert (this->_son1 != NULL);
        if (thePath[0] == '0')
            return (this->_son0->findSonByPath (proximo));
        if (thePath[0] == '1')
            return (this->_son1->findSonByPath (proximo));
    } else {
        assert (this->_son0 == NULL);
        assert (this->_son1 == NULL);
    }
    if (thePath[0] == ' ')
        return (this->findSonByPath (proximo));
    if (thePath[0] == '*')
        if (this->_parent != NULL)
            return (this->_parent->findSonByPath (proximo));
        else
            return (this->findSonByPath (proximo));
    if (thePath[0] == '#')
        return (this->root()->findSonByPath (proximo));
    return (NULL);
}

TCell* TCell::self (void) {
    return (this);
}

std::string TCell::pathFromRootToMe (void) {
    if (this->_parent == NULL)
        return (std::string (""));
    else if (this->_parent->_son0 == this)
        return (this->_parent->pathFromRootToMe() + "0");
    else
        return (this->_parent->pathFromRootToMe() + "1");
}

std::string TCell::pathToSon (TCell &theCellSon) {
    return (this->pathToSon (&theCellSon));
}

std::string TCell::pathToSon (TCell *theCellSon) {
    std::string r = this->_pathToSon (theCellSon);
    if (r != "")
        return (r.substr (0, r.length() - 1));
    return ("");
}

std::string TCell::_pathToSon (TCell *theCellSon) {
    if (theCellSon == NULL)
        return (std::string(""));
    if (theCellSon == this)
        return (std::string(" "));
    else if (this->_hasSons) {
        assert (this->_son0 != NULL);
        assert (this->_son1 != NULL);
        std::string pp = this->_son0->_pathToSon (theCellSon);
        if (pp.length() != 0)
            return (std::string("0") + pp);
        pp = this->_son1->_pathToSon (theCellSon);
        if (pp.length() != 0)
            return (std::string("1") + pp);
    }
    return (std::string(""));
}

bool TCell::isSonOf (TCell &theCellParent) {
    return (this->isSonOf (&theCellParent));
}

bool TCell::isSonOf (TCell *theCellParent) {
    if (theCellParent == this)
        return (false);
    return (this->pathToSon (theCellParent) != "");
}

bool TCell::isParentOf (TCell &theCellSon) {
    return (this->isParentOf (&theCellSon));
}

bool TCell::isParentOf (TCell *theCellSon) {
    if (theCellSon == this || theCellSon == NULL)
        return (false);
    return (theCellSon->pathToSon (this) != "");
}

void TCell::swap (TCell &cell, bool swapParent) {
    this->swap (&cell, swapParent);
}

void TCell::swap (TCell *cell, bool swapParent) {
    if (cell == NULL || cell == this) return;
    this->swapvar (this->_id, cell->_id);
    this->swapvar (this->_weight, cell->_weight);
    this->swapvar (this->_character, cell->_character);
    this->swapvar (this->_son0, cell->_son0);
    this->swapvar (this->_son1, cell->_son1);
    this->swapvar (this->_hasSons, cell->_hasSons);
    if (this->_hasSons) {
        assert (this->_son0 != NULL);
        assert (this->_son1 != NULL);
        this->_son0->_parent = this;
        this->_son1->_parent = this;
    }
    if (cell->_hasSons) {
        assert (cell->_son0 != NULL);
        assert (cell->_son1 != NULL);
        cell->_son0->_parent = cell;
        cell->_son1->_parent = cell;
    }
    if (swapParent) {
        this->swapvar (this->_parent, cell->_parent);
        if (this->_parent != NULL) {
            if (this->_parent->_son0 == cell)
                this->_parent->_son0 = this;
            else {
                assert (this->_parent->_son1 == cell);
                this->_parent->_son1 = this;
            }
        }
        if (cell->_parent != NULL) {
            if (cell->_parent->_son0 == this)
                cell->_parent->_son0 = cell;
            else {
                assert (cell->_parent->_son1 == this);
                cell->_parent->_son1 = cell;
            }
        }
    }
}

void TCell::copyFrom (TCell &cell, bool assignParentToCopy) {
    this->copyFrom (&cell, assignParentToCopy);
}

void TCell::copyFrom (TCell *cell, bool assignParentToCopy) {
    if (cell == NULL || cell == this) return;
    this->_id = cell->_id;
    this->_weight = cell->_weight;
    this->_character = cell->_character;
    if (this->_hasSons)
        this->killSons ();
    if (cell->_hasSons) {
        assert (cell->_son0 != NULL);
        assert (cell->_son1 != NULL);
        this->makeSons ();
        this->_son0->copyFrom (cell->_son0, false);
        this->_son1->copyFrom (cell->_son1, false);
    } else {
        assert (cell->_son0 == NULL);
        assert (cell->_son1 == NULL);
    }
    if (assignParentToCopy) {
        if (this->_parent != NULL) {
            TCell *outro_antes;
            if (this->_parent->_son0 == this)
                outro_antes = this->_parent->_son1;
            else {
                assert (this->_parent->_son1 == this);
                outro_antes = this->_parent->_son0;
            }
            this->_parent->_son0 = NULL;
            this->_parent->_son1 = NULL;
            this->_parent->_hasSons = false;
            this->_parent = NULL;
            outro_antes->_parent = NULL;
            DEL_OBJECT (outro_antes);
        }
        if (cell->_parent != NULL) {
            TCell *parent_antes = cell->_parent;
            if (parent_antes->_son0 == cell)
                parent_antes->_son0 = this;
            else {
                assert (parent_antes->_son1 == cell);
                parent_antes->_son1 = this;
            }
            this->_parent = parent_antes;
            cell->_parent = NULL;
        }
    }
}

void TCell::copyTo (TCell &cell, bool assignParentToCopy) {
    cell.copyFrom (this, assignParentToCopy);
}

void TCell::copyTo (TCell *cell, bool assignParentToCopy) {
    if (cell != NULL)
        cell->copyFrom (this, assignParentToCopy);
}

std::string TCell::dump (int spaces) {
    std::stringstream r;
    if (spaces < 0) spaces = 0;
    std::string r1 (spaces, ' '), r2 (4, ' ');;
    r << r1 << this << std::endl;
    r << r1 << r2 << "character = char(" << int(this->_character) << ")" << std::endl;
    r << r1 << r2 << "id = " << this->_id << std::endl;
    r << r1 << r2 << "weight = " << this->_weight << std::endl;
    if (this->_hasSons) {
        assert (this->_son0 != NULL);
        assert (this->_son1 != NULL);
        r << this->_son0->dump (spaces + r2.length());
        r << this->_son1->dump (spaces + r2.length());
    }
    return (r.str());
}
